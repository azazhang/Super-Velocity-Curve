#include "MidiUtilities.h"

namespace svc
{

void MidiRoutingSettings::setRemap (int sourceNote, int sourceChannel, int targetNote, int targetChannel)
{
    if (sourceNote >= 0 && sourceNote < 128)
    {
        noteRemap[static_cast<size_t> (sourceNote)] = NoteRemapEntry {
            sourceNote, sourceChannel, targetNote, targetChannel
        };
        remapEnabled = true;
    }
}

void MidiRoutingSettings::clearRemaps()
{
    noteRemap.fill (std::nullopt);
    remapEnabled = false;
}

bool MidiRoutingSettings::remapNote (int& note, int& channel) const
{
    if (! remapEnabled || note < 0 || note >= 128)
        return false;

    const auto& entry = noteRemap[static_cast<size_t> (note)];
    if (! entry.has_value())
        return false;

    if (entry->sourceChannel != 0 && entry->sourceChannel != channel)
        return false;

    note = entry->targetNote;
    if (entry->targetChannel != 0)
        channel = entry->targetChannel;

    return true;
}

bool MidiRoutingSettings::passesChannelFilter (int channel) const
{
    return inputChannelFilter == 0 || inputChannelFilter == channel;
}

int MidiRoutingSettings::transformOutputChannel (int channel) const
{
    return outputChannel != 0 ? outputChannel : channel;
}

void MidiRoutingProcessor::setSettings (const MidiRoutingSettings& settings)
{
    routing = settings;
}

void MidiRoutingProcessor::setAftertouchSettings (int note, int channel, const AftertouchPadSettings& settings)
{
    aftertouchPads[{ note, channel }] = settings;
}

void MidiRoutingProcessor::clearAftertouchSettings()
{
    aftertouchPads.clear();
}

float MidiRoutingProcessor::processAftertouch (int note, int channel, float pressure) const
{
    const AtKey key { note, channel };
    const auto it = aftertouchPads.find (key);
    if (it == aftertouchPads.end() || ! it->second.enabled)
        return pressure;

    return it->second.curve.mapNormalized (pressure);
}

bool MidiRoutingProcessor::processMessage (juce::MidiMessage& message) const
{
    auto channel = message.getChannel();
    if (! routing.passesChannelFilter (channel))
        return false;

    if (message.isNoteOn() || message.isNoteOff())
    {
        auto note = message.getNoteNumber();
        routing.remapNote (note, channel);

        const auto outCh = routing.transformOutputChannel (channel);
        if (message.isNoteOn())
            message = juce::MidiMessage::noteOn (outCh, note, message.getFloatVelocity());
        else
            message = juce::MidiMessage::noteOff (outCh, note, message.getFloatVelocity());

        return true;
    }

    if (message.isAftertouch())
    {
        const auto shaped = processAftertouch (message.getNoteNumber(), channel, message.getAfterTouchValue() / 127.0f);
        message = juce::MidiMessage::aftertouchChange (routing.transformOutputChannel (channel),
                                                       message.getNoteNumber(),
                                                       static_cast<int> (shaped * 127.0f));
        return true;
    }

    if (message.isChannelPressure())
    {
        const auto shaped = processAftertouch (0, channel, message.getChannelPressureValue() / 127.0f);
        message = juce::MidiMessage::channelPressureChange (routing.transformOutputChannel (channel),
                                                            static_cast<int> (shaped * 127.0f));
        return true;
    }

    if (routing.outputChannel != 0 && routing.outputChannel != channel && message.isController())
        message = juce::MidiMessage::controllerEvent (routing.outputChannel, message.getControllerNumber(), message.getControllerValue());

    return true;
}

void VelocityHistogram::record (float inputNormalized, float outputNormalized) noexcept
{
    const auto inBin = juce::jlimit (0, 127, static_cast<int> (inputNormalized * 127.0f));
    const auto outBin = juce::jlimit (0, 127, static_cast<int> (outputNormalized * 127.0f));
    inputBins[static_cast<size_t> (inBin)].fetch_add (1, std::memory_order_relaxed);
    outputBins[static_cast<size_t> (outBin)].fetch_add (1, std::memory_order_relaxed);
}

void VelocityHistogram::clear() noexcept
{
    for (auto& bin : inputBins)
        bin.store (0, std::memory_order_relaxed);
    for (auto& bin : outputBins)
        bin.store (0, std::memory_order_relaxed);
}

HistogramSnapshot VelocityHistogram::snapshot() const
{
    HistogramSnapshot copy;
    for (int i = 0; i < 128; ++i)
    {
        copy.inputBins[static_cast<size_t> (i)] = inputBins[static_cast<size_t> (i)].load (std::memory_order_relaxed);
        copy.outputBins[static_cast<size_t> (i)] = outputBins[static_cast<size_t> (i)].load (std::memory_order_relaxed);
    }
    return copy;
}

void HistogramBank::record (int note, int channel, float inputNormalized, float outputNormalized) noexcept
{
    global.record (inputNormalized, outputNormalized);
    perPad[{ note, channel }].record (inputNormalized, outputNormalized);
}

void HistogramBank::clear() noexcept
{
    global.clear();
    perPad.clear();
}

void HistogramBank::clearPad (int note, int channel) noexcept
{
    perPad.erase ({ note, channel });
}

HistogramSnapshot HistogramBank::getGlobalSnapshot() const
{
    return global.snapshot();
}

HistogramSnapshot HistogramBank::getPadSnapshot (int note, int channel) const
{
    const PadHistogramKey key { note, channel };
    const auto it = perPad.find (key);
    return it != perPad.end() ? it->second.snapshot() : HistogramSnapshot {};
}

} // namespace svc

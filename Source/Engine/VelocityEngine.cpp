#include "VelocityEngine.h"
#include "LibraryCompensation.h"

namespace svc
{

VelocityEngine::VelocityEngine() = default;

void VelocityEngine::setSampleRate (double rate) noexcept
{
    sampleRate = rate > 0.0 ? rate : 44100.0;
}

void VelocityEngine::setOutputMode (VelocityOutputMode mode) noexcept
{
    outputMode = mode;
}

void VelocityEngine::clearAllPads()
{
    const std::unique_lock lock (padMutex);
    pads.clear();
    retriggerStates.clear();
    midiRouting.clearAftertouchSettings();
}

void VelocityEngine::setPadSettings (int note, int channel, const PadSettings& settings)
{
    const std::unique_lock lock (padMutex);
    pads[{ note, channel }] = settings;
    if (settings.aftertouch.enabled)
        midiRouting.setAftertouchSettings (note, channel, settings.aftertouch);
}

PadSettings VelocityEngine::getPadSettings (int note, int channel) const
{
    const std::shared_lock lock (padMutex);
    const NoteKey key { note, channel };
    const auto it = pads.find (key);
    if (it != pads.end())
        return it->second;

    PadSettings defaults;
    defaults.midiNote = note;
    defaults.midiChannel = channel;
    defaults.name = "Note " + juce::String (note);
    return defaults;
}

void VelocityEngine::setMidiRouting (const MidiRoutingSettings& settings)
{
    const std::unique_lock lock (padMutex);
    midiRouting.setSettings (settings);
}

const MidiRoutingSettings& VelocityEngine::getMidiRouting() const noexcept
{
    return midiRouting.getSettings();
}

void VelocityEngine::setProcessingSettings (const EngineProcessingSettings& settings)
{
    const std::unique_lock lock (padMutex);
    processingSettings = settings;
}

HistogramSnapshot VelocityEngine::getGlobalHistogramSnapshot() const
{
    return histogramBank.getGlobalSnapshot();
}

HistogramSnapshot VelocityEngine::getPadHistogramSnapshot (int note, int channel) const
{
    return histogramBank.getPadSnapshot (note, channel);
}

const PadSettings* VelocityEngine::findPad (int note, int channel) const
{
    const NoteKey key { note, channel };
    const auto it = pads.find (key);
    return it != pads.end() ? &it->second : nullptr;
}

bool VelocityEngine::shouldDropRetrigger (const PadSettings& pad,
                                          int note,
                                          int channel,
                                          double eventTimeSeconds) noexcept
{
    if (pad.retriggerGuardMs <= 0.0)
        return false;

    const NoteKey key { note, channel };
    const auto it = retriggerStates.find (key);
    if (it == retriggerStates.end() || it->second.lastNoteOnTime < 0.0)
        return false;

    return (eventTimeSeconds - it->second.lastNoteOnTime) < (pad.retriggerGuardMs * 0.001);
}

float VelocityEngine::processNoteVelocity (const PadSettings& pad, float inputNormalized) const
{
    if (! pad.enabled)
        return inputNormalized;

    if (inputNormalized < pad.velocityGate)
    {
        if (pad.gateMode == VelocityGateMode::clampToFloor)
            return pad.curve.mapNormalized (pad.velocityGate);
        return -1.0f;
    }

    auto output = pad.curve.mapNormalized (inputNormalized);
    output = applyLibraryCompensation (output,
                                       processingSettings.libraryPreset,
                                       processingSettings.libraryBlend);
    return applyHumanize (output);
}

float VelocityEngine::applyHumanize (float normalized) const
{
    const auto amount = std::clamp (processingSettings.humanizeAmount, 0.0f, 0.25f);
    if (amount <= 0.0f)
        return normalized;

    const auto spread = amount * 0.5f;
    const auto delta = humanizeRandom.nextFloat() * spread * 2.0f - spread;
    return std::clamp (normalized + delta, 0.0f, 1.0f);
}

int VelocityEngine::resolveOutputChannel (PadGroup group, int incomingChannel) const
{
    if (! processingSettings.zoneRouting.enabled)
        return incomingChannel;

    const auto groupIndex = static_cast<size_t> (group);
    if (groupIndex >= processingSettings.zoneRouting.groupOutputChannel.size())
        return incomingChannel;

    const auto overrideChannel = processingSettings.zoneRouting.groupOutputChannel[groupIndex];
    return overrideChannel > 0 ? overrideChannel : incomingChannel;
}

void VelocityEngine::applyOutputVelocity (juce::MidiMessage& message,
                                          float outputNormalized,
                                          bool inputIsMidi2) const
{
    const auto forceMidi1 = outputMode == VelocityOutputMode::midi1;
    const auto forceMidi2 = outputMode == VelocityOutputMode::midi2;
    const auto useMidi1 = forceMidi1 || (! forceMidi2 && ! inputIsMidi2);

    if (useMidi1)
    {
        const auto midi1 = normalizedToMidi1 (outputNormalized);
        message.setVelocity (midi1ToNormalized (midi1));
    }
    else
    {
        message.setVelocity (outputNormalized);
    }
}

void VelocityEngine::processMidiBuffer (juce::MidiBuffer& buffer, int numSamples)
{
    juce::MidiBuffer processed;
    const auto blockDurationSeconds = static_cast<double> (numSamples) / sampleRate;

    std::shared_lock lock (padMutex);

    for (const auto metadata : buffer)
    {
        auto message = metadata.getMessage();
        const auto sampleOffset = metadata.samplePosition;
        const auto eventTime = runningTimeSeconds + (static_cast<double> (sampleOffset) / sampleRate);

        if (! midiRouting.processMessage (message))
            continue;

        if (message.isNoteOn())
        {
            const auto note = message.getNoteNumber();
            const auto channel = message.getChannel();
            const auto inputNormalized = message.getFloatVelocity();

            const auto midi1Quantized = midi1ToNormalized (message.getVelocity());
            const bool inputIsMidi2 = std::abs (inputNormalized - midi1Quantized) > (1.0f / 254.0f);

            PadSettings settings;
            if (const auto* pad = findPad (note, channel))
                settings = *pad;
            else
            {
                settings.midiNote = note;
                settings.midiChannel = channel;
            }

            if (shouldDropRetrigger (settings, note, channel, eventTime))
                continue;

            const auto outputNormalized = processNoteVelocity (settings, inputNormalized);
            if (outputNormalized < 0.0f)
                continue;

            applyOutputVelocity (message, outputNormalized, inputIsMidi2);

            if (processingSettings.zoneRouting.enabled)
            {
                const auto outChannel = resolveOutputChannel (settings.group, message.getChannel());
                message.setChannel (outChannel);
            }

            histogramBank.record (note, channel, inputNormalized, outputNormalized);

            HitEvent hit;
            hit.note = note;
            hit.channel = channel;
            hit.inputVelocity = inputNormalized;
            hit.outputVelocity = outputNormalized;
            hit.isMidi2 = inputIsMidi2;
            hit.timestamp = static_cast<std::uint64_t> (eventTime * 1000.0);
            hitFifo.push (hit);

            lock.unlock();
            {
                const std::unique_lock writeLock (padMutex);
                retriggerStates[{ note, channel }].lastNoteOnTime = eventTime;
            }
            lock.lock();
        }

        processed.addEvent (message, sampleOffset);
    }

    lock.unlock();
    buffer.swapWith (processed);
    runningTimeSeconds += blockDurationSeconds;
}

} // namespace svc

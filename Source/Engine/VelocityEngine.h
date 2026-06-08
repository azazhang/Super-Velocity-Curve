#pragma once

#include "../Profiles/PadTypes.h"
#include "EngineSettings.h"
#include "HitEventFifo.h"
#include "MidiUtilities.h"
#include "MidiVelocity.h"
#include "VelocityCurve.h"
#include <JuceHeader.h>
#include <shared_mutex>
#include <unordered_map>

namespace svc
{

struct PadSettings
{
    int midiNote = 36;
    int midiChannel = 10;
    juce::String name;
    PadGroup group = PadGroup::other;
    VelocityCurve curve;
    bool enabled = true;
    float velocityGate = 0.0f;
    VelocityGateMode gateMode = VelocityGateMode::drop;
    double retriggerGuardMs = 0.0;
    AftertouchPadSettings aftertouch;
};

class VelocityEngine
{
public:
    VelocityEngine();

    void setSampleRate (double rate) noexcept;
    void setOutputMode (VelocityOutputMode mode) noexcept;
    VelocityOutputMode getOutputMode() const noexcept { return outputMode; }

    void clearAllPads();
    void setPadSettings (int note, int channel, const PadSettings& settings);
    PadSettings getPadSettings (int note, int channel) const;

    void setMidiRouting (const MidiRoutingSettings& settings);
    const MidiRoutingSettings& getMidiRouting() const noexcept;

    void setProcessingSettings (const EngineProcessingSettings& settings);
    const EngineProcessingSettings& getProcessingSettings() const noexcept { return processingSettings; }

    void processMidiBuffer (juce::MidiBuffer& buffer, int numSamples);
    HitEventFifo& getHitFifo() noexcept { return hitFifo; }
    HistogramBank& getHistogramBank() noexcept { return histogramBank; }
    const HistogramBank& getHistogramBank() const noexcept { return histogramBank; }
    HistogramSnapshot getGlobalHistogramSnapshot() const;
    HistogramSnapshot getPadHistogramSnapshot (int note, int channel) const;
    void clearHistogram() noexcept { histogramBank.clear(); }
    void clearPadHistogram (int note, int channel) noexcept { histogramBank.clearPad (note, channel); }

private:
    struct NoteKey
    {
        int note;
        int channel;
        bool operator== (const NoteKey& o) const noexcept { return note == o.note && channel == o.channel; }
    };

    struct NoteKeyHash
    {
        std::size_t operator() (const NoteKey& key) const noexcept
        {
            return static_cast<std::size_t> (key.note) * 17u + static_cast<std::size_t> (key.channel);
        }
    };

    struct RetriggerState { double lastNoteOnTime = -1.0; };

    using PadMap = std::unordered_map<NoteKey, PadSettings, NoteKeyHash>;
    using RetriggerMap = std::unordered_map<NoteKey, RetriggerState, NoteKeyHash>;

    PadMap pads;
    RetriggerMap retriggerStates;
    MidiRoutingProcessor midiRouting;
    EngineProcessingSettings processingSettings;
    mutable std::shared_mutex padMutex;
    mutable juce::Random humanizeRandom;
    VelocityOutputMode outputMode = VelocityOutputMode::autoDetect;
    HitEventFifo hitFifo;
    HistogramBank histogramBank;
    double sampleRate = 44100.0;
    double runningTimeSeconds = 0.0;

    const PadSettings* findPad (int note, int channel) const;
    float processNoteVelocity (const PadSettings& pad, float inputNormalized) const;
    float applyHumanize (float normalized) const;
    int resolveOutputChannel (PadGroup group, int incomingChannel) const;
    void applyOutputVelocity (juce::MidiMessage& message, float outputNormalized, bool inputIsMidi2) const;
    bool shouldDropRetrigger (const PadSettings& pad, int note, int channel, double eventTimeSeconds) noexcept;
};

} // namespace svc

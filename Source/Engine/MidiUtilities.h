#pragma once

#include "VelocityCurve.h"
#include <JuceHeader.h>
#include <array>
#include <atomic>
#include <memory>
#include <optional>
#include <unordered_map>

namespace svc
{

inline constexpr std::size_t kMidiChannels = 16;
inline constexpr std::size_t kMidiNotes = 128;
inline constexpr std::size_t kMidiNoteChannelSlots = kMidiChannels * kMidiNotes;

inline std::size_t midiNoteChannelIndex (int note, int channel) noexcept
{
    const auto clampedNote = juce::jlimit (0, static_cast<int> (kMidiNotes) - 1, note);
    const auto clampedChannel = juce::jlimit (1, static_cast<int> (kMidiChannels), channel);
    return static_cast<std::size_t> (clampedNote + (clampedChannel - 1) * static_cast<int> (kMidiNotes));
}

struct NoteRemapEntry
{
    int sourceNote = 0;
    int sourceChannel = 0;
    int targetNote = 0;
    int targetChannel = 0;
};

struct MidiRoutingSettings
{
    int inputChannelFilter = 0;
    int outputChannel = 0;
    bool remapEnabled = false;
    std::array<std::optional<NoteRemapEntry>, 128> noteRemap {};

    void setRemap (int sourceNote, int sourceChannel, int targetNote, int targetChannel);
    void clearRemaps();
    bool remapNote (int& note, int& channel) const;
    bool passesChannelFilter (int channel) const;
    int transformOutputChannel (int channel) const;
};

struct AftertouchPadSettings
{
    bool enabled = false;
    VelocityCurve curve;
};

class MidiRoutingProcessor
{
public:
    void setSettings (const MidiRoutingSettings& settings);
    const MidiRoutingSettings& getSettings() const noexcept { return routing; }

    void setAftertouchSettings (int note, int channel, const AftertouchPadSettings& settings);
    void clearAftertouchSettings();
    bool processMessage (juce::MidiMessage& message) const;

private:
    MidiRoutingSettings routing;
    struct AtKey { int note; int channel; bool operator== (const AtKey& o) const noexcept { return note == o.note && channel == o.channel; } };
    struct AtHash { std::size_t operator() (const AtKey& k) const noexcept { return static_cast<std::size_t> (k.note) * 17u + static_cast<std::size_t> (k.channel); } };
    std::unordered_map<AtKey, AftertouchPadSettings, AtHash> aftertouchPads;

    static constexpr int kChannelPressureNote = -1;

    float processAftertouch (int note, int channel, float pressure) const;
    float processChannelPressure (int channel, float pressure) const;
};

struct HistogramSnapshot
{
    std::array<int, 128> inputBins {};
    std::array<int, 128> outputBins {};
};

struct VelocityHistogram
{
    std::array<std::atomic<int>, 128> inputBins {};
    std::array<std::atomic<int>, 128> outputBins {};

    void record (float inputNormalized, float outputNormalized) noexcept;
    void clear() noexcept;
    HistogramSnapshot snapshot() const;
};

class HistogramBank
{
public:
    HistogramBank();

    void record (int note, int channel, float inputNormalized, float outputNormalized) noexcept;
    void clear() noexcept;
    void clearPad (int note, int channel) noexcept;
    HistogramSnapshot getGlobalSnapshot() const;
    HistogramSnapshot getPadSnapshot (int note, int channel) const;

private:
    VelocityHistogram global;
    // Heap-backed: ~2 MiB of atomics must not live inline on VelocityEngine (Windows stack is 1 MiB).
    std::unique_ptr<std::array<VelocityHistogram, kMidiNoteChannelSlots>> perPad;
};

} // namespace svc

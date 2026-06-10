#pragma once

#include <JuceHeader.h>

namespace svc::ui
{

inline juce::String formatMidiNote (int note) noexcept
{
    const auto clamped = juce::jlimit (0, 127, note);
    return juce::String (clamped) + " (" + juce::MidiMessage::getMidiNoteName (clamped, true, true, 3) + ")";
}

/** Short name for tight pad cells (e.g. Launchpad 8-column grid). */
inline juce::String formatMidiNoteShort (int note) noexcept
{
    const auto clamped = juce::jlimit (0, 127, note);
    return juce::MidiMessage::getMidiNoteName (clamped, true, true, 3);
}

} // namespace svc::ui

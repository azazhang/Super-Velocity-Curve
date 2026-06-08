#pragma once

#include <JuceHeader.h>
#include <vector>

namespace svc
{

struct DrumPadDefinition
{
    int midiNote;
    juce::String name;
    int row;
    int col;
};

// Finger-drumming oriented layout: 4 columns, primary kit in rows 0-3, extras below.
inline const std::vector<DrumPadDefinition>& getGMDrumPads()
{
    static const std::vector<DrumPadDefinition> pads = {
        { 46, "Open HH",    0, 0 },
        { 42, "Closed HH",  0, 1 },
        { 44, "Pedal HH",   0, 2 },
        { 51, "Ride",       0, 3 },
        { 50, "High Tom",   1, 0 },
        { 48, "Hi-Mid Tom", 1, 1 },
        { 45, "Low Tom",    1, 2 },
        { 49, "Crash",      1, 3 },
        { 37, "Side Stick", 2, 0 },
        { 38, "Snare",      2, 1 },
        { 39, "Hand Clap",  2, 2 },
        { 40, "E-Snare",    2, 3 },
        { 36, "Kick",       3, 0 },
        { 43, "Floor Tom",  3, 1 },
        { 41, "Low Floor",  3, 2 },
        { 47, "Low-Mid",    3, 3 },
        { 52, "China",      4, 0 },
        { 55, "Splash",     4, 1 },
        { 57, "Crash 2",    4, 2 },
        { 59, "Ride 2",     4, 3 },
        { 53, "Ride Bell",  5, 0 },
        { 54, "Tambourine", 5, 1 },
        { 56, "Cowbell",    5, 2 },
        { 58, "Vibraslap",  5, 3 },
        { 35, "Acoustic BD", 6, 0 }
    };
    return pads;
}

inline juce::String getGMDrumName (int midiNote)
{
    for (const auto& pad : getGMDrumPads())
        if (pad.midiNote == midiNote)
            return pad.name;

    if (midiNote >= 35 && midiNote <= 81)
        return "GM Perc " + juce::String (midiNote);

    return "Note " + juce::String (midiNote);
}

} // namespace svc

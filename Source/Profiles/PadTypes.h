#pragma once

#include <JuceHeader.h>

namespace svc
{

enum class PadGroup
{
    other,
    kick,
    snare,
    hat,
    tom,
    cymbal,
    percussion
};

enum class VelocityGateMode
{
    drop,
    clampToFloor
};

inline juce::String padGroupToString (PadGroup group)
{
    switch (group)
    {
        case PadGroup::kick:       return "Kick";
        case PadGroup::snare:      return "Snare";
        case PadGroup::hat:        return "Hat";
        case PadGroup::tom:        return "Tom";
        case PadGroup::cymbal:     return "Cymbal";
        case PadGroup::percussion: return "Percussion";
        case PadGroup::other:
        default:                   return "Other";
    }
}

inline PadGroup padGroupFromString (const juce::String& text)
{
    if (text == "Kick")       return PadGroup::kick;
    if (text == "Snare")      return PadGroup::snare;
    if (text == "Hat")        return PadGroup::hat;
    if (text == "Tom")        return PadGroup::tom;
    if (text == "Cymbal")     return PadGroup::cymbal;
    if (text == "Percussion") return PadGroup::percussion;
    return PadGroup::other;
}

inline juce::String gateModeToString (VelocityGateMode mode)
{
    return mode == VelocityGateMode::clampToFloor ? "Clamp to floor" : "Drop notes";
}

inline VelocityGateMode gateModeFromString (const juce::String& text)
{
    return text == "Clamp to floor" ? VelocityGateMode::clampToFloor : VelocityGateMode::drop;
}

} // namespace svc

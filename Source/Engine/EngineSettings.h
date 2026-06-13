#pragma once

#include "../Profiles/PadTypes.h"
#include <array>
#include <string_view>

namespace svc
{

enum class LibraryCompensationPreset
{
    none,
    acoustic,
    electronic,
    compressed
};

inline const char* libraryCompensationPresetName (LibraryCompensationPreset preset) noexcept
{
    switch (preset)
    {
        case LibraryCompensationPreset::acoustic:    return "Acoustic samples";
        case LibraryCompensationPreset::electronic:  return "Electronic samples";
        case LibraryCompensationPreset::compressed:  return "Compressed samples";
        case LibraryCompensationPreset::none:
        default:                                     return "None";
    }
}

inline LibraryCompensationPreset libraryCompensationPresetFromName (std::string_view name) noexcept
{
    if (name == "Acoustic samples" || name == "Acoustic library")
        return LibraryCompensationPreset::acoustic;
    if (name == "Electronic samples" || name == "Electronic library")
        return LibraryCompensationPreset::electronic;
    if (name == "Compressed samples" || name == "Compressed library")
        return LibraryCompensationPreset::compressed;
    return LibraryCompensationPreset::none;
}

struct ZoneRoutingSettings
{
    /** Per PadGroup output channel override. 0 = keep incoming channel. */
    std::array<int, 7> groupOutputChannel {};
    bool enabled = false;
};

struct EngineProcessingSettings
{
    float humanizeAmount = 0.0f;
    LibraryCompensationPreset libraryPreset = LibraryCompensationPreset::none;
    float libraryBlend = 0.0f;
    ZoneRoutingSettings zoneRouting;
};

} // namespace svc

#pragma once

#include "EngineSettings.h"
#include "VelocityCurve.h"

namespace svc
{

inline std::pair<const CurveControlPoint*, size_t> libraryCompensationPointSpan (LibraryCompensationPreset preset) noexcept
{
    static constexpr CurveControlPoint acoustic[] = {
        { 0.0f, 0.0f }, { 0.35f, 0.28f }, { 0.65f, 0.62f }, { 1.0f, 1.0f }
    };
    static constexpr CurveControlPoint electronic[] = {
        { 0.0f, 0.0f }, { 0.25f, 0.42f }, { 0.55f, 0.72f }, { 1.0f, 1.0f }
    };
    static constexpr CurveControlPoint compressed[] = {
        { 0.0f, 0.0f }, { 0.4f, 0.55f }, { 0.7f, 0.82f }, { 1.0f, 1.0f }
    };
    static constexpr CurveControlPoint linear[] = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };

    switch (preset)
    {
        case LibraryCompensationPreset::acoustic:   return { acoustic, std::size (acoustic) };
        case LibraryCompensationPreset::electronic: return { electronic, std::size (electronic) };
        case LibraryCompensationPreset::compressed: return { compressed, std::size (compressed) };
        case LibraryCompensationPreset::none:
        default:                                    return { linear, std::size (linear) };
    }
}

inline float applyLibraryCompensation (float normalized,
                                       LibraryCompensationPreset preset,
                                       float blend) noexcept
{
    const auto amount = std::clamp (blend, 0.0f, 1.0f);
    if (amount <= 0.0f || preset == LibraryCompensationPreset::none)
        return normalized;

    const auto [points, count] = libraryCompensationPointSpan (preset);
    const auto compensated = VelocityCurve::interpolateControlPoints (points, count, normalized);
    return normalized + amount * (compensated - normalized);
}

} // namespace svc

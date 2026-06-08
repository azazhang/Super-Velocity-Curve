#pragma once

#include "EngineSettings.h"
#include "VelocityCurve.h"
#include <vector>

namespace svc
{

inline std::vector<CurveControlPoint> libraryCompensationPoints (LibraryCompensationPreset preset)
{
    switch (preset)
    {
        case LibraryCompensationPreset::acoustic:
            return { { 0.0f, 0.0f }, { 0.35f, 0.28f }, { 0.65f, 0.62f }, { 1.0f, 1.0f } };
        case LibraryCompensationPreset::electronic:
            return { { 0.0f, 0.0f }, { 0.25f, 0.42f }, { 0.55f, 0.72f }, { 1.0f, 1.0f } };
        case LibraryCompensationPreset::compressed:
            return { { 0.0f, 0.0f }, { 0.4f, 0.55f }, { 0.7f, 0.82f }, { 1.0f, 1.0f } };
        case LibraryCompensationPreset::none:
        default:
            return { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
    }
}

inline float applyLibraryCompensation (float normalized,
                                       LibraryCompensationPreset preset,
                                       float blend) noexcept
{
    const auto amount = std::clamp (blend, 0.0f, 1.0f);
    if (amount <= 0.0f || preset == LibraryCompensationPreset::none)
        return normalized;

    VelocityCurve curve;
    curve.setControlPoints (libraryCompensationPoints (preset));
    const auto compensated = curve.mapNormalized (normalized);
    return normalized + amount * (compensated - normalized);
}

} // namespace svc

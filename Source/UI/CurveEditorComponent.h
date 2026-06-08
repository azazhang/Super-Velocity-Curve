#pragma once

#include "../Profiles/ControllerProfile.h"
#include "Theme.h"
#include <JuceHeader.h>
#include <functional>
#include <vector>

class CurveEditorComponent : public juce::Component
{
public:
    CurveEditorComponent();

    void setPad (const svc::ProfilePad& pad, bool clearHitMarkers = true);
    const svc::ProfilePad& getPad() const noexcept { return currentPad; }

    enum class EditTarget { velocity, aftertouch };

    void setEditTarget (EditTarget target);
    void addHitMarker (int note, int channel, float inputNormalized, float outputNormalized, bool isMidi2);

    std::function<void (const svc::ProfilePad&)> onPadChanged;

    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;
    void mouseDoubleClick (const juce::MouseEvent& event) override;

    void applyPreset (svc::CurvePreset preset);
    void resetCurve();
    void copyFrom (const svc::VelocityCurve& other);
    void setFloorCeiling (float floor, float ceiling);

    /** Ghost overlay for A/B compare (does not change editable pad). */
    void setCompareCurve (const svc::VelocityCurve* curve) noexcept { compareCurve = curve; repaint(); }
    void clearCompareCurve() noexcept { compareCurve = nullptr; repaint(); }

private:
    svc::ProfilePad currentPad;
    const svc::VelocityCurve* compareCurve = nullptr;

    struct HitMarker
    {
        float input = 0.0f;
        float output = 0.0f;
        double createdMs = 0.0;
    };

    std::vector<HitMarker> hitMarkers;
    int draggedPointIndex = -1;
    EditTarget editTarget = EditTarget::velocity;

    juce::Rectangle<float> plotArea() const;
    juce::Point<float> normalizedToPoint (float input, float output) const;
    juce::Point<float> eventToNormalized (juce::Point<float> pos) const;
    int findNearestControlPoint (juce::Point<float> pos) const;
    void notifyChanged();
    void drawGrid (juce::Graphics& g) const;
    void drawGateZones (juce::Graphics& g) const;
    void drawCurve (juce::Graphics& g) const;
    void drawCurvePath (juce::Graphics& g, const svc::VelocityCurve& curve, juce::Colour colour, float strokeWidth) const;
    svc::VelocityCurve& activeCurve() noexcept;
    const svc::VelocityCurve& activeCurve() const noexcept;
};

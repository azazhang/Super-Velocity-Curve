#pragma once

#include "../Engine/VelocityCurve.h"
#include "Theme.h"
#include <JuceHeader.h>
#include <functional>
#include <vector>

class CalibrationWizardComponent : public juce::Component
{
public:
    CalibrationWizardComponent();

    std::function<void (const svc::VelocityCurve&)> onCurveCalibrated;

    void captureHit (float velocityNormalized);
    void reset();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    enum class Step { soft, medium, hard, done };

    Step step = Step::soft;
    float softHit = -1.0f;
    float mediumHit = -1.0f;
    float hardHit = -1.0f;

    juce::TextButton resetButton { "Reset" };
    juce::TextButton applyButton { "Apply Curve" };
    juce::Label instructionLabel;

    void advanceStep();
    svc::VelocityCurve buildCurve() const;
    juce::String instructionText() const;
    void drawPreviewCurve (juce::Graphics& g, juce::Rectangle<float> area) const;
};

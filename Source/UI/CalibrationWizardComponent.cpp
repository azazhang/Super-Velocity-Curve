#include "CalibrationWizardComponent.h"

CalibrationWizardComponent::CalibrationWizardComponent()
{
    addAndMakeVisible (instructionLabel);
    addAndMakeVisible (resetButton);
    addAndMakeVisible (applyButton);

    instructionLabel.setJustificationType (juce::Justification::topLeft);
    instructionLabel.setFont (svc::ui::Theme::bodyFont());

    resetButton.onClick = [this] { reset(); };
    applyButton.onClick = [this]
    {
        if (onCurveCalibrated && step == Step::done)
            onCurveCalibrated (buildCurve());
    };

    reset();
}

void CalibrationWizardComponent::reset()
{
    step = Step::soft;
    softHit = mediumHit = hardHit = -1.0f;
    instructionLabel.setText (instructionText(), juce::dontSendNotification);
    repaint();
}

juce::String CalibrationWizardComponent::instructionText() const
{
    switch (step)
    {
        case Step::soft:   return "Step 1/3: Hit selected pad softly (ghost note).";
        case Step::medium: return "Step 2/3: Hit with medium force.";
        case Step::hard:   return "Step 3/3: Hit with maximum accent force.";
        case Step::done:   return "Calibration complete. Preview below, then Apply.";
    }
    return {};
}

void CalibrationWizardComponent::captureHit (float velocityNormalized)
{
    switch (step)
    {
        case Step::soft:
            softHit = velocityNormalized;
            step = Step::medium;
            break;
        case Step::medium:
            mediumHit = velocityNormalized;
            step = Step::hard;
            break;
        case Step::hard:
            hardHit = velocityNormalized;
            step = Step::done;
            break;
        case Step::done:
            return;
    }

    instructionLabel.setText (instructionText(), juce::dontSendNotification);
    repaint();
}

svc::VelocityCurve CalibrationWizardComponent::buildCurve() const
{
    svc::VelocityCurve curve;
    if (softHit < 0.0f || mediumHit < 0.0f || hardHit < 0.0f)
        return curve;

    const auto s = juce::jlimit (0.0f, 1.0f, softHit);
    const auto m = juce::jlimit (s + 0.01f, 1.0f, mediumHit);
    const auto h = juce::jlimit (m + 0.01f, 1.0f, hardHit);

    std::vector<svc::CurveControlPoint> points = {
        { 0.0f, 0.0f },
        { s, 0.25f },
        { m, 0.55f },
        { h, 0.92f },
        { 1.0f, 1.0f }
    };
    curve.setControlPoints (std::move (points));
    return curve;
}

void CalibrationWizardComponent::drawPreviewCurve (juce::Graphics& g, juce::Rectangle<float> area) const
{
    if (step != Step::done)
        return;

    const auto curve = buildCurve();
    const auto& lut = curve.getLut();
    juce::Path path;
    for (int i = 0; i < svc::VelocityCurve::lutSize; ++i)
    {
        const auto x = area.getX() + area.getWidth() * static_cast<float> (i) / static_cast<float> (svc::VelocityCurve::lutSize - 1);
        const auto y = area.getBottom() - lut[static_cast<size_t> (i)] * area.getHeight();
        if (i == 0)
            path.startNewSubPath (x, y);
        else
            path.lineTo (x, y);
    }

    g.setColour (juce::Colour (svc::ui::Theme::curveLine));
    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void CalibrationWizardComponent::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 8.0f);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary));
    g.setFont (svc::ui::Theme::sectionFont());
    g.drawText ("Calibration Wizard", getLocalBounds().removeFromTop (22).reduced (10, 0), juce::Justification::centredLeft);

    auto info = getLocalBounds().reduced (10).withTrimmedTop (52).withTrimmedBottom (30);
    g.setFont (svc::ui::Theme::smallFont());
    g.setColour (juce::Colour (svc::ui::Theme::textSecondary));
    if (softHit >= 0.0f)
        g.drawText ("Soft: " + juce::String (static_cast<int> (softHit * 127.0f)), info.removeFromTop (14), juce::Justification::left);
    if (mediumHit >= 0.0f)
        g.drawText ("Medium: " + juce::String (static_cast<int> (mediumHit * 127.0f)), info.removeFromTop (14), juce::Justification::left);
    if (hardHit >= 0.0f)
        g.drawText ("Hard: " + juce::String (static_cast<int> (hardHit * 127.0f)), info.removeFromTop (14), juce::Justification::left);

    if (step == Step::done)
    {
        auto preview = info.removeFromTop (juce::jmax (40, info.getHeight()));
        g.setColour (juce::Colour (svc::ui::Theme::background));
        g.fillRoundedRectangle (preview.toFloat(), 4.0f);
        drawPreviewCurve (g, preview.reduced (4).toFloat());
    }
}

void CalibrationWizardComponent::resized()
{
    auto area = getLocalBounds().reduced (10).withTrimmedTop (24);
    instructionLabel.setBounds (area.removeFromTop (48));
    area.removeFromTop (40);
    auto buttons = area.removeFromBottom (26);
    resetButton.setBounds (buttons.removeFromLeft (buttons.getWidth() / 2).reduced (2));
    applyButton.setBounds (buttons.reduced (2));
}

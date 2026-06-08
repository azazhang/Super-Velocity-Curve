#include "PadInspectorComponent.h"

PadInspectorComponent::PadInspectorComponent()
{
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&content, false);
    viewport.setScrollBarsShown (true, false);

    content.addAndMakeVisible (enabledToggle);
    enabledToggle.onClick = [this] { currentPad.enabled = enabledToggle.getToggleState(); notifyChanged(); };

    for (auto* label : { &gateLabel, &retriggerLabel, &floorLabel, &ceilingLabel })
        content.addAndMakeVisible (label);

    setupSlider (velocityGateSlider, "");
    setupSlider (retriggerSlider, " ms");
    setupSlider (floorSlider, "");
    setupSlider (ceilingSlider, "");

    velocityGateSlider.setRange (0.0, 1.0, 0.01);
    retriggerSlider.setRange (0.0, 100.0, 1.0);
    floorSlider.setRange (0.0, 1.0, 0.01);
    ceilingSlider.setRange (0.0, 1.0, 0.01);

    velocityGateSlider.onValueChange = [this]
    {
        currentPad.velocityGate = static_cast<float> (velocityGateSlider.getValue());
        notifyChanged();
    };

    retriggerSlider.onValueChange = [this]
    {
        currentPad.retriggerGuardMs = retriggerSlider.getValue();
        notifyChanged();
    };

    floorSlider.onValueChange = [this]
    {
        currentPad.curve.setFloor (static_cast<float> (floorSlider.getValue()));
        notifyChanged();
    };

    ceilingSlider.onValueChange = [this]
    {
        currentPad.curve.setCeiling (static_cast<float> (ceilingSlider.getValue()));
        notifyChanged();
    };

    for (auto* slider : { &velocityGateSlider, &retriggerSlider, &floorSlider, &ceilingSlider })
        content.addAndMakeVisible (slider);
}

void PadInspectorComponent::setupSlider (juce::Slider& slider, const juce::String& suffix)
{
    slider.setTextValueSuffix (suffix);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 52, 16);
}

void PadInspectorComponent::setPad (const svc::ProfilePad& pad, int padIndex)
{
    currentPad = pad;
    currentPadIndex = padIndex;

    enabledToggle.setToggleState (pad.enabled, juce::dontSendNotification);
    velocityGateSlider.setValue (pad.velocityGate, juce::dontSendNotification);
    retriggerSlider.setValue (pad.retriggerGuardMs, juce::dontSendNotification);
    floorSlider.setValue (pad.curve.getFloor(), juce::dontSendNotification);
    ceilingSlider.setValue (pad.curve.getCeiling(), juce::dontSendNotification);
}

void PadInspectorComponent::notifyChanged()
{
    if (onPadChanged)
        onPadChanged (currentPadIndex, currentPad);
}

void PadInspectorComponent::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 10.0f);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary));
    g.setFont (svc::ui::Theme::sectionFont());
    g.drawText ("Pad Settings", getLocalBounds().removeFromTop (28).reduced (12, 0), juce::Justification::centredLeft);
}

void PadInspectorComponent::resized()
{
    auto area = getLocalBounds().reduced (8).withTrimmedTop (28);
    viewport.setBounds (area);

    content.setSize (area.getWidth() - 8, 250);

    auto bounds = content.getLocalBounds().reduced (8);
    enabledToggle.setBounds (bounds.removeFromTop (22));
    bounds.removeFromTop (8);

    auto row = [&bounds] (juce::Label& label, juce::Slider& slider)
    {
        auto r = bounds.removeFromTop (54);
        label.setBounds (r.removeFromTop (14));
        slider.setBounds (r);
        bounds.removeFromTop (6);
    };

    row (gateLabel, velocityGateSlider);
    row (retriggerLabel, retriggerSlider);

    auto pair = bounds.removeFromTop (54);
    auto left = pair.removeFromLeft (pair.getWidth() / 2).reduced (0, 0);
    auto right = pair.reduced (0, 0);
    floorLabel.setBounds (left.removeFromTop (14));
    floorSlider.setBounds (left);
    ceilingLabel.setBounds (right.removeFromTop (14));
    ceilingSlider.setBounds (right);
}

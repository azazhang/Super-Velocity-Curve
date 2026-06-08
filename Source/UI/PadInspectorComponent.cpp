#include "PadInspectorComponent.h"

PadInspectorComponent::PadInspectorComponent()
{
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&content, false);
    viewport.setScrollBarsShown (true, false);

    content.addAndMakeVisible (enabledToggle);
    enabledToggle.onClick = [this] { currentPad.enabled = enabledToggle.getToggleState(); notifyChanged(); };

    content.addAndMakeVisible (aftertouchToggle);
    aftertouchToggle.onClick = [this]
    {
        currentPad.aftertouch.enabled = aftertouchToggle.getToggleState();
        notifyChanged();
    };

    content.addAndMakeVisible (editAftertouchButton);
    editAftertouchButton.onClick = [this]
    {
        if (onEditAftertouchRequested)
            onEditAftertouchRequested();
    };

    for (auto* label : { &gateLabel, &gateModeLabel, &groupLabel, &retriggerLabel, &floorLabel, &ceilingLabel })
        content.addAndMakeVisible (label);

    content.addAndMakeVisible (groupBox);
    for (int i = 0; i < 7; ++i)
        groupBox.addItem (svc::padGroupToString (static_cast<svc::PadGroup> (i)), i + 1);
    groupBox.onChange = [this]
    {
        currentPad.group = static_cast<svc::PadGroup> (groupBox.getSelectedItemIndex());
        notifyChanged();
    };

    content.addAndMakeVisible (gateModeBox);
    gateModeBox.addItem ("Drop notes", 1);
    gateModeBox.addItem ("Clamp to floor", 2);
    gateModeBox.onChange = [this]
    {
        currentPad.gateMode = gateModeBox.getSelectedId() == 2 ? svc::VelocityGateMode::clampToFloor
                                                               : svc::VelocityGateMode::drop;
        notifyChanged();
    };

    setupSlider (velocityGateSlider, "", true);
    setupSlider (retriggerSlider, " ms", false);
    setupSlider (floorSlider, "", true);
    setupSlider (ceilingSlider, "", true);

    velocityGateSlider.setRange (0.0, 127.0, 1.0);
    retriggerSlider.setRange (0.0, 100.0, 1.0);
    floorSlider.setRange (0.0, 127.0, 1.0);
    ceilingSlider.setRange (0.0, 127.0, 1.0);

    velocityGateSlider.onValueChange = [this]
    {
        currentPad.velocityGate = static_cast<float> (velocityGateSlider.getValue() / 127.0);
        notifyChanged();
    };

    retriggerSlider.onValueChange = [this]
    {
        currentPad.retriggerGuardMs = retriggerSlider.getValue();
        notifyChanged();
    };

    floorSlider.onValueChange = [this]
    {
        currentPad.curve.setFloor (static_cast<float> (floorSlider.getValue() / 127.0));
        notifyChanged();
    };

    ceilingSlider.onValueChange = [this]
    {
        currentPad.curve.setCeiling (static_cast<float> (ceilingSlider.getValue() / 127.0));
        notifyChanged();
    };

    for (auto* slider : { &velocityGateSlider, &retriggerSlider, &floorSlider, &ceilingSlider })
        content.addAndMakeVisible (slider);
}

void PadInspectorComponent::setupSlider (juce::Slider& slider, const juce::String& suffix, bool midiScale)
{
    slider.setTextValueSuffix (suffix);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 52, 16);
    if (midiScale)
    {
        slider.textFromValueFunction = [] (double value) { return juce::String (static_cast<int> (value)); };
        slider.valueFromTextFunction = [] (const juce::String& text) { return text.getDoubleValue(); };
    }
}

void PadInspectorComponent::setPad (const svc::ProfilePad& pad, int padIndex)
{
    currentPad = pad;
    currentPadIndex = padIndex;

    enabledToggle.setToggleState (pad.enabled, juce::dontSendNotification);
    aftertouchToggle.setToggleState (pad.aftertouch.enabled, juce::dontSendNotification);
    groupBox.setSelectedItemIndex (static_cast<int> (pad.group), juce::dontSendNotification);
    gateModeBox.setSelectedId (pad.gateMode == svc::VelocityGateMode::clampToFloor ? 2 : 1, juce::dontSendNotification);
    velocityGateSlider.setValue (pad.velocityGate * 127.0, juce::dontSendNotification);
    retriggerSlider.setValue (pad.retriggerGuardMs, juce::dontSendNotification);
    floorSlider.setValue (pad.curve.getFloor() * 127.0, juce::dontSendNotification);
    ceilingSlider.setValue (pad.curve.getCeiling() * 127.0, juce::dontSendNotification);
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
    content.setSize (area.getWidth() - 8, 360);

    auto bounds = content.getLocalBounds().reduced (8);
    enabledToggle.setBounds (bounds.removeFromTop (22));
    aftertouchToggle.setBounds (bounds.removeFromTop (22));
    editAftertouchButton.setBounds (bounds.removeFromTop (24));
    bounds.removeFromTop (6);

    auto row = [&bounds] (juce::Label& label, juce::Component& comp, int h = 44)
    {
        auto r = bounds.removeFromTop (h);
        label.setBounds (r.removeFromTop (14));
        comp.setBounds (r);
        bounds.removeFromTop (4);
    };

    row (groupLabel, groupBox, 40);
    row (gateLabel, velocityGateSlider);
    row (gateModeLabel, gateModeBox, 40);
    row (retriggerLabel, retriggerSlider);

    auto pair = bounds.removeFromTop (54);
    auto left = pair.removeFromLeft (pair.getWidth() / 2);
    auto right = pair;
    floorLabel.setBounds (left.removeFromTop (14));
    floorSlider.setBounds (left);
    ceilingLabel.setBounds (right.removeFromTop (14));
    ceilingSlider.setBounds (right);
}

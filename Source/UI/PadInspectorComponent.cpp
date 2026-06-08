#include "PadInspectorComponent.h"

PadInspectorComponent::PadInspectorComponent()
{
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&content, false);
    viewport.setScrollBarsShown (true, false);
    viewport.getVerticalScrollBar().setAutoHide (true);

    padNameEditor.setFont (svc::ui::Theme::bodyFont());
    padNameEditor.setTextToShowWhenEmpty ("Pad name", juce::Colour (svc::ui::Theme::textSecondary()));
    padNameEditor.onFocusLost = padNameEditor.onReturnKey = [this]
    {
        currentPad.label = padNameEditor.getText().trim();
        if (currentPad.label.isEmpty())
            currentPad.label = "Pad " + juce::String (currentPadIndex + 1);
        notifyChanged();
    };
    content.addAndMakeVisible (padNameLabel);
    content.addAndMakeVisible (padNameEditor);

    midiNoteSlider.setRange (0.0, 127.0, 1.0);
    midiNoteSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 52, 16);
    midiNoteSlider.textFromValueFunction = [] (double v) { return juce::String (static_cast<int> (v)); };
    midiNoteSlider.onValueChange = [this]
    {
        currentPad.midiNote = static_cast<int> (midiNoteSlider.getValue());
        notifyChanged();
    };
    content.addAndMakeVisible (midiNoteLabel);
    content.addAndMakeVisible (midiNoteSlider);

    for (int ch = 1; ch <= 16; ++ch)
        midiChannelBox.addItem ("Ch " + juce::String (ch), ch);
    midiChannelBox.onChange = [this]
    {
        currentPad.midiChannel = midiChannelBox.getSelectedId();
        notifyChanged();
    };
    content.addAndMakeVisible (midiChannelLabel);
    content.addAndMakeVisible (midiChannelBox);

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

    applyTheme();
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

svc::ProfilePad PadInspectorComponent::getPad() const
{
    auto pad = currentPad;
    pad.label = padNameEditor.getText().trim();
    if (pad.label.isEmpty())
        pad.label = "Pad " + juce::String (currentPadIndex + 1);
    return pad;
}

void PadInspectorComponent::commitEdits()
{
    if (suppressNotify)
        return;

    currentPad = getPad();
    notifyChanged();
}

void PadInspectorComponent::applyTheme()
{
    const auto text = juce::Colour (svc::ui::Theme::textPrimary());
    const auto bg = juce::Colour (svc::ui::Theme::panelRaised());
    const auto border = juce::Colour (svc::ui::Theme::border());

    for (auto* label : { &padNameLabel, &midiNoteLabel, &midiChannelLabel, &gateLabel, &gateModeLabel,
                         &groupLabel, &retriggerLabel, &floorLabel, &ceilingLabel })
        label->setColour (juce::Label::textColourId, text);

    padNameEditor.setColour (juce::TextEditor::textColourId, text);
    padNameEditor.setColour (juce::TextEditor::backgroundColourId, bg);
    padNameEditor.setColour (juce::TextEditor::outlineColourId, border);
    padNameEditor.setColour (juce::TextEditor::focusedOutlineColourId,
                             juce::Colour (svc::ui::Theme::accent()).withAlpha (0.55f));
    padNameEditor.setColour (juce::CaretComponent::caretColourId, juce::Colour (svc::ui::Theme::accentGold()));
}

void PadInspectorComponent::setPad (const svc::ProfilePad& pad, int padIndex)
{
    suppressNotify = true;
    currentPad = pad;
    currentPadIndex = padIndex;

    padNameEditor.setText (pad.label, juce::dontSendNotification);
    midiNoteSlider.setValue (pad.midiNote, juce::dontSendNotification);
    midiChannelBox.setSelectedId (juce::jlimit (1, 16, pad.midiChannel), juce::dontSendNotification);
    enabledToggle.setToggleState (pad.enabled, juce::dontSendNotification);
    aftertouchToggle.setToggleState (pad.aftertouch.enabled, juce::dontSendNotification);
    groupBox.setSelectedItemIndex (static_cast<int> (pad.group), juce::dontSendNotification);
    gateModeBox.setSelectedId (pad.gateMode == svc::VelocityGateMode::clampToFloor ? 2 : 1, juce::dontSendNotification);
    velocityGateSlider.setValue (pad.velocityGate * 127.0, juce::dontSendNotification);
    retriggerSlider.setValue (pad.retriggerGuardMs, juce::dontSendNotification);
    floorSlider.setValue (pad.curve.getFloor() * 127.0, juce::dontSendNotification);
    ceilingSlider.setValue (pad.curve.getCeiling() * 127.0, juce::dontSendNotification);
    suppressNotify = false;
}

void PadInspectorComponent::notifyChanged()
{
    if (suppressNotify || ! onPadChanged)
        return;

    onPadChanged (currentPadIndex, getPad());
}

void PadInspectorComponent::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 10.0f);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary()));
    g.setFont (svc::ui::Theme::sectionFont());
    g.drawText ("Pad Settings", getLocalBounds().removeFromTop (28).reduced (12, 0), juce::Justification::centredLeft);
}

void PadInspectorComponent::resized()
{
    auto area = getLocalBounds().reduced (8).withTrimmedTop (28);
    viewport.setBounds (area);
    layoutContent();
}

void PadInspectorComponent::layoutContent()
{
    const int width = juce::jmax (220, viewport.getWidth() - 4);
    int y = 8;

    auto row = [&y, width] (juce::Label& label, juce::Component& comp, int h = 44)
    {
        label.setBounds (8, y, width - 16, 14);
        comp.setBounds (8, y + 14, width - 16, h - 14);
        y += h + 4;
    };

    row (padNameLabel, padNameEditor, 40);
    row (midiNoteLabel, midiNoteSlider);
    row (midiChannelLabel, midiChannelBox, 40);
    enabledToggle.setBounds (8, y, width - 16, 22);
    y += 24;
    aftertouchToggle.setBounds (8, y, width - 16, 22);
    y += 24;
    editAftertouchButton.setBounds (8, y, width - 16, 24);
    y += 30;

    row (groupLabel, groupBox, 40);
    row (gateLabel, velocityGateSlider);
    row (gateModeLabel, gateModeBox, 40);
    row (retriggerLabel, retriggerSlider);

    auto pairY = y;
    floorLabel.setBounds (8, pairY, (width - 20) / 2, 14);
    ceilingLabel.setBounds (8 + (width - 20) / 2 + 4, pairY, (width - 20) / 2, 14);
    floorSlider.setBounds (8, pairY + 14, (width - 20) / 2, 40);
    ceilingSlider.setBounds (8 + (width - 20) / 2 + 4, pairY + 14, (width - 20) / 2, 40);
    y = pairY + 58;

    content.setSize (width, y + 8);
}

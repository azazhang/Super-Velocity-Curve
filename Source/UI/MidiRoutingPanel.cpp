#include "MidiRoutingPanel.h"

MidiRoutingPanel::MidiRoutingPanel()
{
    addAndMakeVisible (inputLabel);
    addAndMakeVisible (outputLabel);
    addAndMakeVisible (inputChannelBox);
    addAndMakeVisible (outputChannelBox);
    addAndMakeVisible (remapToggle);

    for (int ch = 0; ch <= 16; ++ch)
    {
        inputChannelBox.addItem (ch == 0 ? "Any" : "Ch " + juce::String (ch), ch + 1);
        outputChannelBox.addItem (ch == 0 ? "Keep original" : "Ch " + juce::String (ch), ch + 1);
    }

    inputChannelBox.onChange = [this] { notifyChanged(); };
    outputChannelBox.onChange = [this] { notifyChanged(); };
    remapToggle.onClick = [this] { notifyChanged(); };
}

void MidiRoutingPanel::setProfile (svc::ControllerProfile& p)
{
    profile = &p;
    const auto& routing = profile->getMidiRouting();
    inputChannelBox.setSelectedId (routing.inputChannelFilter + 1, juce::dontSendNotification);
    outputChannelBox.setSelectedId (routing.outputChannel + 1, juce::dontSendNotification);
    remapToggle.setToggleState (routing.remapEnabled, juce::dontSendNotification);
}

void MidiRoutingPanel::notifyChanged()
{
    if (profile == nullptr)
        return;

    auto& routing = profile->getMidiRouting();
    routing.inputChannelFilter = inputChannelBox.getSelectedId() - 1;
    routing.outputChannel = outputChannelBox.getSelectedId() - 1;
    routing.remapEnabled = remapToggle.getToggleState();

    if (onRoutingChanged)
        onRoutingChanged();
}

void MidiRoutingPanel::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 8.0f);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary));
    g.setFont (svc::ui::Theme::sectionFont());
    g.drawText ("MIDI Routing", getLocalBounds().removeFromTop (22).reduced (10, 0), juce::Justification::centredLeft);
}

void MidiRoutingPanel::resized()
{
    auto area = getLocalBounds().reduced (10).withTrimmedTop (24);
    inputLabel.setBounds (area.removeFromTop (14));
    inputChannelBox.setBounds (area.removeFromTop (22));
    area.removeFromTop (4);
    outputLabel.setBounds (area.removeFromTop (14));
    outputChannelBox.setBounds (area.removeFromTop (22));
    area.removeFromTop (4);
    remapToggle.setBounds (area.removeFromTop (22));
}

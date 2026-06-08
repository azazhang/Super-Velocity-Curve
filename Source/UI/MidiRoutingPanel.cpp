#include "MidiRoutingPanel.h"
#include "../Engine/EngineSettings.h"

namespace
{
constexpr const char* groupLabels[] = { "Other", "Kick", "Snare", "Hat", "Tom", "Cymbal", "Percussion" };
}

MidiRoutingPanel::MidiRoutingPanel()
{
    addAndMakeVisible (inputLabel);
    addAndMakeVisible (outputLabel);
    addAndMakeVisible (inputChannelBox);
    addAndMakeVisible (outputChannelBox);
    addAndMakeVisible (remapToggle);
    addAndMakeVisible (humanizeLabel);
    addAndMakeVisible (humanizeSlider);
    addAndMakeVisible (libraryLabel);
    addAndMakeVisible (libraryPresetBox);
    addAndMakeVisible (libraryBlendSlider);
    addAndMakeVisible (zoneRoutingToggle);

    zoneRoutingToggle.setButtonText ("Zone routing (per pad group channel)");

    humanizeLabel.setText ("Humanize amount", juce::dontSendNotification);
    libraryLabel.setText ("Library compensation", juce::dontSendNotification);
    humanizeSlider.setRange (0.0, 0.25, 0.001);
    libraryBlendSlider.setRange (0.0, 1.0, 0.01);

    for (int ch = 0; ch <= 16; ++ch)
    {
        inputChannelBox.addItem (ch == 0 ? "Any" : "Ch " + juce::String (ch), ch + 1);
        outputChannelBox.addItem (ch == 0 ? "Keep original" : "Ch " + juce::String (ch), ch + 1);
    }

    libraryPresetBox.addItem ("None", 1);
    libraryPresetBox.addItem ("Acoustic library", 2);
    libraryPresetBox.addItem ("Electronic library", 3);
    libraryPresetBox.addItem ("Compressed library", 4);

    inputChannelBox.onChange = [this] { notifyChanged(); };
    outputChannelBox.onChange = [this] { notifyChanged(); };
    remapToggle.onClick = [this] { notifyChanged(); };
    humanizeSlider.onValueChange = [this] { notifyChanged(); };
    libraryPresetBox.onChange = [this] { notifyChanged(); };
    libraryBlendSlider.onValueChange = [this] { notifyChanged(); };
    zoneRoutingToggle.onClick = [this] { notifyChanged(); };
}

void MidiRoutingPanel::setProfile (svc::ControllerProfile& p)
{
    profile = &p;
    const auto& routing = profile->getMidiRouting();
    const auto& processing = profile->getProcessingSettings();

    inputChannelBox.setSelectedId (routing.inputChannelFilter + 1, juce::dontSendNotification);
    outputChannelBox.setSelectedId (routing.outputChannel + 1, juce::dontSendNotification);
    remapToggle.setToggleState (routing.remapEnabled, juce::dontSendNotification);
    humanizeSlider.setValue (processing.humanizeAmount, juce::dontSendNotification);
    libraryBlendSlider.setValue (processing.libraryBlend, juce::dontSendNotification);
    zoneRoutingToggle.setToggleState (processing.zoneRouting.enabled, juce::dontSendNotification);

    int presetId = 1;
    switch (processing.libraryPreset)
    {
        case svc::LibraryCompensationPreset::acoustic:    presetId = 2; break;
        case svc::LibraryCompensationPreset::electronic:  presetId = 3; break;
        case svc::LibraryCompensationPreset::compressed:  presetId = 4; break;
        case svc::LibraryCompensationPreset::none:      presetId = 1; break;
    }
    libraryPresetBox.setSelectedId (presetId, juce::dontSendNotification);
}

void MidiRoutingPanel::notifyChanged()
{
    if (profile == nullptr)
        return;

    auto& routing = profile->getMidiRouting();
    routing.inputChannelFilter = inputChannelBox.getSelectedId() - 1;
    routing.outputChannel = outputChannelBox.getSelectedId() - 1;
    routing.remapEnabled = remapToggle.getToggleState();

    auto& processing = profile->getProcessingSettings();
    processing.humanizeAmount = static_cast<float> (humanizeSlider.getValue());
    processing.libraryBlend = static_cast<float> (libraryBlendSlider.getValue());
    processing.zoneRouting.enabled = zoneRoutingToggle.getToggleState();

    switch (libraryPresetBox.getSelectedId())
    {
        case 2:  processing.libraryPreset = svc::LibraryCompensationPreset::acoustic; break;
        case 3:  processing.libraryPreset = svc::LibraryCompensationPreset::electronic; break;
        case 4:  processing.libraryPreset = svc::LibraryCompensationPreset::compressed; break;
        default: processing.libraryPreset = svc::LibraryCompensationPreset::none; break;
    }

    if (onRoutingChanged)
        onRoutingChanged();
}

void MidiRoutingPanel::paint (juce::Graphics& g)
{
    juce::ignoreUnused (g);
}

void MidiRoutingPanel::resized()
{
    auto area = getLocalBounds().reduced (8);
    inputLabel.setBounds (area.removeFromTop (14));
    inputChannelBox.setBounds (area.removeFromTop (22));
    area.removeFromTop (4);
    outputLabel.setBounds (area.removeFromTop (14));
    outputChannelBox.setBounds (area.removeFromTop (22));
    area.removeFromTop (4);
    remapToggle.setBounds (area.removeFromTop (22));
    area.removeFromTop (6);
    humanizeLabel.setBounds (area.removeFromTop (14));
    humanizeSlider.setBounds (area.removeFromTop (22));
    area.removeFromTop (4);
    libraryLabel.setBounds (area.removeFromTop (14));
    libraryPresetBox.setBounds (area.removeFromTop (22));
    area.removeFromTop (2);
    libraryBlendSlider.setBounds (area.removeFromTop (22));
    area.removeFromTop (4);
    zoneRoutingToggle.setBounds (area.removeFromTop (22));
}

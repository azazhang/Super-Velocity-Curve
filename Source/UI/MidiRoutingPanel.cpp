#include "MidiRoutingPanel.h"
#include "../Engine/EngineSettings.h"
#include "../Profiles/PadTypes.h"

namespace
{
constexpr const char* groupLabels[] = { "Other", "Kick", "Snare", "Hat", "Tom", "Cymbal", "Percussion" };
}

MidiRoutingPanel::MidiRoutingPanel()
{
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&content, false);
    viewport.setScrollBarsShown (true, false);
    viewport.getVerticalScrollBar().setAutoHide (true);

    content.addAndMakeVisible (inputLabel);
    content.addAndMakeVisible (outputLabel);
    content.addAndMakeVisible (inputChannelBox);
    content.addAndMakeVisible (outputChannelBox);
    content.addAndMakeVisible (remapToggle);
    content.addAndMakeVisible (humanizeLabel);
    content.addAndMakeVisible (humanizeSlider);
    content.addAndMakeVisible (libraryLabel);
    content.addAndMakeVisible (libraryPresetBox);
    content.addAndMakeVisible (libraryBlendSlider);
    content.addAndMakeVisible (zoneRoutingToggle);
    content.addAndMakeVisible (zoneChannelsLabel);

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

    for (int i = 0; i < 7; ++i)
    {
        zoneGroupLabels[static_cast<size_t> (i)].setText (groupLabels[i], juce::dontSendNotification);
        content.addAndMakeVisible (zoneGroupLabels[static_cast<size_t> (i)]);
        content.addAndMakeVisible (zoneGroupChannelBoxes[static_cast<size_t> (i)]);

        for (int ch = 0; ch <= 16; ++ch)
            zoneGroupChannelBoxes[static_cast<size_t> (i)].addItem (ch == 0 ? "Keep" : "Ch " + juce::String (ch), ch + 1);

        zoneGroupChannelBoxes[static_cast<size_t> (i)].onChange = [this] { notifyChanged(); };
    }

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

    for (int i = 0; i < 7; ++i)
    {
        const auto ch = processing.zoneRouting.groupOutputChannel[static_cast<size_t> (i)];
        zoneGroupChannelBoxes[static_cast<size_t> (i)].setSelectedId (ch + 1, juce::dontSendNotification);
    }

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

    for (int i = 0; i < 7; ++i)
        processing.zoneRouting.groupOutputChannel[static_cast<size_t> (i)] =
            zoneGroupChannelBoxes[static_cast<size_t> (i)].getSelectedId() - 1;

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

void MidiRoutingPanel::layoutContent()
{
    auto bounds = juce::Rectangle<int> (0, 0, juce::jmax (viewport.getWidth(), 280), 520).reduced (4);

    auto row = [&bounds] (juce::Component& comp, int h)
    {
        comp.setBounds (bounds.removeFromTop (h));
        bounds.removeFromTop (4);
    };

    auto labelRow = [&bounds] (juce::Label& label, juce::Component& comp, int h = 40)
    {
        auto r = bounds.removeFromTop (h);
        label.setBounds (r.removeFromTop (14));
        comp.setBounds (r);
        bounds.removeFromTop (4);
    };

    labelRow (inputLabel, inputChannelBox);
    labelRow (outputLabel, outputChannelBox);
    row (remapToggle, 22);
    bounds.removeFromTop (4);
    labelRow (humanizeLabel, humanizeSlider);
    labelRow (libraryLabel, libraryPresetBox);
    row (libraryBlendSlider, 22);
    row (zoneRoutingToggle, 22);
    row (zoneChannelsLabel, 16);

    for (int i = 0; i < 7; ++i)
    {
        auto r = bounds.removeFromTop (40);
        zoneGroupLabels[static_cast<size_t> (i)].setBounds (r.removeFromTop (14));
        zoneGroupChannelBoxes[static_cast<size_t> (i)].setBounds (r);
        bounds.removeFromTop (2);
    }

    content.setSize (bounds.getWidth() + 8, bounds.getY() + 8);
}

void MidiRoutingPanel::paint (juce::Graphics& g)
{
    juce::ignoreUnused (g);
}

void MidiRoutingPanel::resized()
{
    viewport.setBounds (getLocalBounds());
    layoutContent();
}

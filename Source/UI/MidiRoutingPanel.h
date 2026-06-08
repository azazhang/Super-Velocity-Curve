#pragma once

#include "../Profiles/ControllerProfile.h"
#include "Theme.h"
#include <JuceHeader.h>
#include <functional>

class MidiRoutingPanel : public juce::Component
{
public:
    MidiRoutingPanel();

    void setProfile (svc::ControllerProfile& profile);
    std::function<void()> onRoutingChanged;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    svc::ControllerProfile* profile = nullptr;

    juce::ComboBox inputChannelBox;
    juce::ComboBox outputChannelBox;
    juce::ToggleButton remapToggle { "Enable note remap" };
    juce::Label inputLabel { {}, "Input channel (0=any)" };
    juce::Label outputLabel { {}, "Output channel (0=keep)" };
    juce::Label humanizeLabel;
    juce::Slider humanizeSlider { juce::Slider::LinearHorizontal, juce::Slider::TextBoxLeft };
    juce::Label libraryLabel;
    juce::ComboBox libraryPresetBox;
    juce::Slider libraryBlendSlider { juce::Slider::LinearHorizontal, juce::Slider::TextBoxLeft };
    juce::ToggleButton zoneRoutingToggle { "Zone routing" };

    void notifyChanged();
};

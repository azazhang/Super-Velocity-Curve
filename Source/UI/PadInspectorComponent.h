#pragma once

#include "../Profiles/ControllerProfile.h"
#include "Theme.h"
#include <JuceHeader.h>
#include <functional>

class PadInspectorComponent : public juce::Component
{
public:
    PadInspectorComponent();

    void setPad (const svc::ProfilePad& pad, int padIndex);
    svc::ProfilePad getPad() const;
    void commitEdits();
    void applyTheme();
    int getPadIndex() const noexcept { return currentPadIndex; }

    std::function<void (int padIndex, const svc::ProfilePad&)> onPadChanged;
    std::function<void()> onEditAftertouchRequested;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    svc::ProfilePad currentPad;
    int currentPadIndex = 0;

    juce::Viewport viewport;
    juce::Component content;

    juce::Label padNameLabel { {}, "Pad name" };
    juce::TextEditor padNameEditor;
    juce::Label midiNoteLabel { {}, "MIDI note (0-127)" };
    juce::Slider midiNoteSlider;
    juce::Label midiChannelLabel { {}, "MIDI channel" };
    juce::ComboBox midiChannelBox;
    juce::ToggleButton enabledToggle { "Pad enabled" };
    juce::ToggleButton aftertouchToggle { "Aftertouch curve enabled" };
    juce::TextButton editAftertouchButton { "Edit AT curve" };
    juce::ComboBox groupBox;
    juce::ComboBox gateModeBox;
    juce::Slider velocityGateSlider;
    juce::Slider retriggerSlider;
    juce::Slider floorSlider;
    juce::Slider ceilingSlider;
    juce::Label gateLabel { {}, "Velocity gate" };
    juce::Label gateModeLabel { {}, "Gate mode" };
    juce::Label groupLabel { {}, "Pad group" };
    juce::Label retriggerLabel { {}, "Retrigger guard (ms)" };
    juce::Label floorLabel { {}, "Output floor" };
    juce::Label ceilingLabel { {}, "Output ceiling" };

    void notifyChanged();
    void setupSlider (juce::Slider& slider, const juce::String& suffix, bool midiScale);
    void layoutContent();

    bool suppressNotify = false;
};

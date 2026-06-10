#pragma once

#include "Theme.h"
#include <JuceHeader.h>
#include <functional>

/** In-plugin modal — native AlertWindow freezes/crashes in many DAW hosts. */
class UnsavedProfileDialogComponent : public juce::Component
{
public:
    enum class Choice { save, discard, cancel };

    UnsavedProfileDialogComponent();

    std::function<void (Choice)> onChoice;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Rectangle<int> cardBounds;
    juce::Label titleLabel;
    juce::Label messageLabel;
    juce::TextButton saveButton { "Save" };
    juce::TextButton discardButton { "Discard" };
    juce::TextButton cancelButton { "Cancel" };

    void finish (Choice choice);
};

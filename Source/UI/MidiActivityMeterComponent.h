#pragma once

#include "Theme.h"
#include <JuceHeader.h>

class MidiActivityMeterComponent : public juce::Component
{
public:
    MidiActivityMeterComponent();
    void pushInputLevel (float velocityNormalized);
    void pushOutputLevel (float velocityNormalized);
    void decay();
    bool hasVisibleLevels() const noexcept;
    void paint (juce::Graphics& g) override;

private:
    float inputLevel = 0.0f;
    float outputLevel = 0.0f;
};

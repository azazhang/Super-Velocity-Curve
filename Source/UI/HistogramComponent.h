#pragma once

#include "../Engine/MidiUtilities.h"
#include "Theme.h"
#include <JuceHeader.h>

class HistogramComponent : public juce::Component
{
public:
    void setHistogram (const svc::HistogramSnapshot& histogram);
    void setTitle (const juce::String& title);
    void paint (juce::Graphics& g) override;

private:
    svc::HistogramSnapshot data;
    juce::String title { "Velocity Histogram" };
};

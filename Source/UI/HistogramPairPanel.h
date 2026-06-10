#pragma once

#include "HistogramComponent.h"
#include <JuceHeader.h>

class HistogramPairPanel : public juce::Component
{
public:
    HistogramPairPanel (HistogramComponent& padHistogramIn, HistogramComponent& globalHistogramIn);

    void resized() override;

private:
    HistogramComponent& padHistogram;
    HistogramComponent& globalHistogram;
};

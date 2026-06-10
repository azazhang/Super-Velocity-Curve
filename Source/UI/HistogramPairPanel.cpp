#include "HistogramPairPanel.h"

HistogramPairPanel::HistogramPairPanel (HistogramComponent& padHistogramIn,
                                        HistogramComponent& globalHistogramIn)
    : padHistogram (padHistogramIn),
      globalHistogram (globalHistogramIn)
{
    addAndMakeVisible (padHistogram);
    addAndMakeVisible (globalHistogram);
}

void HistogramPairPanel::resized()
{
    auto area = getLocalBounds();
    padHistogram.setBounds (area.removeFromLeft (area.getWidth() / 2).reduced (2));
    globalHistogram.setBounds (area.reduced (2));
}

#include "HistogramComponent.h"

HistogramComponent::HistogramComponent()
{
    setOpaque (true);
}

void HistogramComponent::setHistogram (const svc::HistogramSnapshot& histogram)
{
    if (data.inputBins == histogram.inputBins && data.outputBins == histogram.outputBins)
        return;

    data = histogram;
    repaint();
}

void HistogramComponent::setTitle (const juce::String& newTitle)
{
    title = newTitle;
    repaint();
}

void HistogramComponent::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 8.0f);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary()));
    g.setFont (svc::ui::Theme::sectionFont());
    g.drawText (title, getLocalBounds().removeFromTop (22).reduced (10, 0), juce::Justification::centredLeft);

    const auto plot = getLocalBounds().reduced (10).withTrimmedTop (24).toFloat();
    if (plot.getWidth() < 10.0f || plot.getHeight() < 10.0f)
        return;

    g.setColour (juce::Colour (svc::ui::Theme::accent()).withAlpha (0.12f));
    const auto bandLeft = plot.getX() + plot.getWidth() * (60.0f / 127.0f);
    const auto bandRight = plot.getX() + plot.getWidth() * (90.0f / 127.0f);
    g.fillRect (bandLeft, plot.getY(), bandRight - bandLeft, plot.getHeight());

    int maxCount = 1;
    for (int i = 0; i < 128; ++i)
    {
        maxCount = juce::jmax (maxCount,
                               data.inputBins[static_cast<size_t> (i)],
                               data.outputBins[static_cast<size_t> (i)]);
    }

    const auto barW = plot.getWidth() / 128.0f;
    for (int i = 0; i < 128; ++i)
    {
        const auto x = plot.getX() + static_cast<float> (i) * barW;
        const auto inCount = data.inputBins[static_cast<size_t> (i)];
        const auto outCount = data.outputBins[static_cast<size_t> (i)];
        const auto inH = plot.getHeight() * static_cast<float> (inCount) / static_cast<float> (maxCount);
        const auto outH = plot.getHeight() * static_cast<float> (outCount) / static_cast<float> (maxCount);

        g.setColour (juce::Colour (svc::ui::Theme::accentWarm()).withAlpha (0.55f));
        g.fillRect (x, plot.getBottom() - inH, barW * 0.45f, inH);
        g.setColour (juce::Colour (svc::ui::Theme::accent()).withAlpha (0.75f));
        g.fillRect (x + barW * 0.5f, plot.getBottom() - outH, barW * 0.45f, outH);
    }

    g.setFont (svc::ui::Theme::smallFont());
    g.setColour (juce::Colour (svc::ui::Theme::textMuted()));
    g.drawText ("60-90", static_cast<int> (bandLeft), static_cast<int> (plot.getY()) - 12, 40, 12, juce::Justification::centred);
    g.drawText ("In (warm) / Out (blue)  |  band = typical touch range (vel 60-90)",
                getLocalBounds().removeFromBottom (14).reduced (10, 0),
                juce::Justification::centredLeft);
}

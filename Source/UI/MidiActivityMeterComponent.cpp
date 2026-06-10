#include "MidiActivityMeterComponent.h"

void MidiActivityMeterComponent::pushInputLevel (float velocityNormalized)
{
    inputLevel = juce::jmax (inputLevel, velocityNormalized);
}

void MidiActivityMeterComponent::pushOutputLevel (float velocityNormalized)
{
    outputLevel = juce::jmax (outputLevel, velocityNormalized);
}

void MidiActivityMeterComponent::decay()
{
    const bool hadActivity = inputLevel > 0.01f || outputLevel > 0.01f;

    inputLevel *= 0.85f;
    outputLevel *= 0.85f;
    if (inputLevel < 0.01f) inputLevel = 0.0f;
    if (outputLevel < 0.01f) outputLevel = 0.0f;

    if (hadActivity)
        repaint();
}

void MidiActivityMeterComponent::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 6.0f);
    g.setFont (svc::ui::Theme::smallFont());
    g.setColour (juce::Colour (svc::ui::Theme::textSecondary()));
    auto area = getLocalBounds().reduced (8);
    g.drawText ("MIDI I/O", area.removeFromTop (14), juce::Justification::centredLeft);

    auto drawMeter = [&g] (juce::Rectangle<int> bounds, float level, juce::Colour colour, const juce::String& label)
    {
        g.setColour (juce::Colour (svc::ui::Theme::textSecondary()));
        g.drawText (label, bounds.removeFromLeft (36), juce::Justification::centredLeft);
        g.setColour (juce::Colour (svc::ui::Theme::panelRaised()).darker (0.08f));
        g.fillRoundedRectangle (bounds.toFloat(), 3.0f);
        auto filled = bounds.withWidth (juce::roundToInt (static_cast<float> (bounds.getWidth()) * level));
        g.setColour (colour);
        g.fillRoundedRectangle (filled.toFloat(), 3.0f);
    };

    auto inRow = area.removeFromTop (14);
    drawMeter (inRow, inputLevel, juce::Colour (svc::ui::Theme::accentWarm()), "In");
    area.removeFromTop (4);
    drawMeter (area, outputLevel, juce::Colour (svc::ui::Theme::accent()), "Out");
}

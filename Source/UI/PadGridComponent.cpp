#include "PadGridComponent.h"

PadGridComponent::PadCanvas::PadCanvas (PadGridComponent& o) : owner (o) {}

void PadGridComponent::PadCanvas::paint (juce::Graphics& g)
{
    const auto& pads = owner.currentProfile.getPads();

    for (int i = 0; i < static_cast<int> (pads.size()); ++i)
    {
        auto bounds = owner.padBoundsForIndex (i);
        const auto& pad = pads[static_cast<size_t> (i)];

        juce::uint32 baseColour = svc::ui::Theme::padIdle();
        if (! pad.enabled)
            baseColour = svc::ui::Theme::padDisabled();
        else if (i == owner.selectedPadIndex)
            baseColour = svc::ui::Theme::padSelected();
        else if (i == owner.hoveredPadIndex)
            baseColour = svc::ui::Theme::padHover();

        const auto hitIt = owner.hitByPadIndex.find (i);
        if (hitIt != owner.hitByPadIndex.end())
        {
            const auto blend = juce::jlimit (0.0f, 1.0f, hitIt->second.intensity);
            baseColour = juce::Colour (baseColour).interpolatedWith (juce::Colour (svc::ui::Theme::padHit()), blend).getARGB();
        }

        const auto rect = bounds.toFloat();
        juce::ColourGradient grad (juce::Colour (baseColour).brighter (i == owner.selectedPadIndex ? 0.12f : 0.06f),
                                   rect.getCentreX(), rect.getY(),
                                   juce::Colour (baseColour).darker (0.08f),
                                   rect.getCentreX(), rect.getBottom(),
                                   false);
        g.setGradientFill (grad);
        g.fillRoundedRectangle (rect, 7.0f);

        if (i == owner.selectedPadIndex)
        {
            g.setColour (juce::Colour (svc::ui::Theme::accent()).withAlpha (0.35f));
            g.drawRoundedRectangle (rect.expanded (0.5f), 7.5f, 1.2f);
        }

        g.setColour (juce::Colour (svc::ui::Theme::border()).withAlpha (0.9f));
        g.drawRoundedRectangle (rect, 7.0f, 1.0f);

        auto textArea = bounds.reduced (6);
        g.setColour (pad.enabled ? juce::Colour (svc::ui::Theme::textPrimary())
                                 : juce::Colour (svc::ui::Theme::textSecondary()));
        g.setFont (svc::ui::Theme::bodyFont().boldened());
        g.drawFittedText (pad.label, textArea.removeFromTop (textArea.getHeight() / 2), juce::Justification::centred, 2);

        g.setFont (svc::ui::Theme::smallFont());
        g.setColour (juce::Colour (svc::ui::Theme::textSecondary()));
        g.drawText ("N" + juce::String (pad.midiNote) + "  Ch" + juce::String (pad.midiChannel),
                    textArea,
                    juce::Justification::centred);

        if (pad.retriggerGuardMs > 0.0)
        {
            g.setColour (juce::Colour (svc::ui::Theme::accentWarm()).withAlpha (0.85f));
            g.fillEllipse (static_cast<float> (bounds.getRight()) - 12.0f,
                           static_cast<float> (bounds.getY()) + 4.0f,
                           8.0f,
                           8.0f);
        }
    }
}

void PadGridComponent::PadCanvas::mouseDown (const juce::MouseEvent& event)
{
    const auto index = owner.padIndexAt (event.getPosition());
    if (index >= 0)
    {
        owner.setSelectedPadIndex (index);
        if (owner.onPadSelected)
            owner.onPadSelected (index);
    }
}

void PadGridComponent::PadCanvas::mouseMove (const juce::MouseEvent& event)
{
    const auto index = owner.padIndexAt (event.getPosition());
    if (index != owner.hoveredPadIndex)
    {
        owner.hoveredPadIndex = index;
        repaint();
    }
}

PadGridComponent::PadGridComponent()
    : padCanvas (*this)
{
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&padCanvas, false);
    viewport.setScrollBarsShown (true, false);
}

void PadGridComponent::setProfile (const svc::ControllerProfile& profile)
{
    currentProfile = profile;
    selectedPadIndex = juce::jlimit (0, juce::jmax (0, static_cast<int> (profile.getPads().size()) - 1), selectedPadIndex);
    hitByPadIndex.clear();
    updateCanvasSize();
    padCanvas.repaint();
    repaint();
}

void PadGridComponent::setSelectedPadIndex (int index)
{
    selectedPadIndex = juce::jlimit (0, juce::jmax (0, static_cast<int> (currentProfile.getPads().size()) - 1), index);
    padCanvas.repaint();
}

void PadGridComponent::flashPadHit (int note, int channel, float outputVelocity)
{
    const auto& pads = currentProfile.getPads();
    for (int i = 0; i < static_cast<int> (pads.size()); ++i)
    {
        const auto& pad = pads[static_cast<size_t> (i)];
        if (pad.midiNote == note && pad.midiChannel == channel)
        {
            hitByPadIndex[i] = { outputVelocity, juce::Time::getMillisecondCounterHiRes() };
            padCanvas.repaint();
            return;
        }
    }
}

void PadGridComponent::decayHitVisuals()
{
    const auto now = juce::Time::getMillisecondCounterHiRes();
    bool changed = false;

    for (auto it = hitByPadIndex.begin(); it != hitByPadIndex.end();)
    {
        const auto age = now - it->second.lastUpdateMs;
        if (age > 500.0)
        {
            it = hitByPadIndex.erase (it);
            changed = true;
        }
        else
        {
            it->second.intensity = juce::jmax (0.0f, it->second.intensity - 0.08f);
            ++it;
            changed = true;
        }
    }

    if (changed)
        padCanvas.repaint();
}

int PadGridComponent::cellWidth() const
{
    return 96;
}

int PadGridComponent::cellHeight() const
{
    return 68;
}

void PadGridComponent::updateCanvasSize()
{
    int maxDisplayRow = 0;
    for (const auto& pad : currentProfile.getPads())
        maxDisplayRow = juce::jmax (maxDisplayRow, pad.gridRow + (pad.gridCol / 4));

    padCanvas.setSize (juce::jmax (4 * cellWidth() + 16, viewport.getWidth()),
                       (maxDisplayRow + 1) * cellHeight() + 16);
}

juce::Rectangle<int> PadGridComponent::padBoundsForIndex (int index) const
{
    const auto& pads = currentProfile.getPads();
    if (index < 0 || index >= static_cast<int> (pads.size()))
        return {};

    const auto& pad = pads[static_cast<size_t> (index)];
    const int cols = 4;
    const int displayCol = pad.gridCol % cols;
    const int displayRow = pad.gridRow + (pad.gridCol / cols);

    return { 8 + displayCol * cellWidth(),
             8 + displayRow * cellHeight(),
             cellWidth() - 6,
             cellHeight() - 6 };
}

int PadGridComponent::padIndexAt (juce::Point<int> pos) const
{
    const auto& pads = currentProfile.getPads();

    for (int i = static_cast<int> (pads.size()) - 1; i >= 0; --i)
    {
        if (padBoundsForIndex (i).contains (pos))
            return i;
    }

    return -1;
}

void PadGridComponent::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 10.0f);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary()));
    g.setFont (svc::ui::Theme::sectionFont());
    g.drawText ("Pad Layout", getLocalBounds().removeFromTop (28).reduced (12, 0), juce::Justification::centredLeft);
}

void PadGridComponent::resized()
{
    auto area = getLocalBounds().reduced (8).withTrimmedTop (28);
    viewport.setBounds (area);
    updateCanvasSize();
}

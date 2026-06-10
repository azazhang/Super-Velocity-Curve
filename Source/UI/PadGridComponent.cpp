#include "PadGridComponent.h"
#include "MidiNoteNames.h"
#include "ScrollHelpers.h"

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
        if (owner.displayGridColumns >= 8)
        {
            g.drawFittedText (svc::ui::formatMidiNoteShort (pad.midiNote),
                            textArea.removeFromTop (textArea.getHeight() / 2),
                            juce::Justification::centred,
                            1);
            g.drawFittedText ("Ch " + juce::String (pad.midiChannel),
                            textArea,
                            juce::Justification::centred,
                            1);
        }
        else
        {
            g.drawText (svc::ui::formatMidiNote (pad.midiNote) + "  Ch" + juce::String (pad.midiChannel),
                        textArea,
                        juce::Justification::centred);
        }

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
    if (index >= 0 && owner.onPadSelected)
        owner.onPadSelected (index);
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
    addAndMakeVisible (addPadButton);
    addAndMakeVisible (deletePadButton);
    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&padCanvas, false);
    svc::ui::configurePadGridViewport (viewport);

    addPadButton.onClick = [this]
    {
        if (onAddPadRequested)
            onAddPadRequested();
    };

    deletePadButton.onClick = [this]
    {
        if (onDeletePadRequested)
            onDeletePadRequested();
    };
}

void PadGridComponent::setProfile (const svc::ControllerProfile& profile, bool resetSelection)
{
    currentProfile = profile;
    displayGridColumns = profile.getDisplayGridColumns();

    if (resetSelection)
        selectedPadIndex = profile.getPads().empty() ? -1 : 0;
    else
        selectedPadIndex = juce::jlimit (-1, juce::jmax (0, static_cast<int> (profile.getPads().size()) - 1),
                                       selectedPadIndex);

    hitByPadIndex.clear();
    updateCanvasSize();
    svc::ui::updatePadGridScrollbars (viewport, padCanvas);
    padCanvas.repaint();
    repaint();
}

void PadGridComponent::scrollPadIntoView (int index)
{
    const auto bounds = padBoundsForIndex (index);
    if (! bounds.isEmpty())
        viewport.setViewPosition (bounds.getX() - 8, bounds.getY() - 8);
}

void PadGridComponent::updatePad (int index, const svc::ProfilePad& pad)
{
    if (index < 0 || index >= static_cast<int> (currentProfile.getPads().size()))
        return;

    currentProfile.getPads()[static_cast<size_t> (index)] = pad;
    padCanvas.repaint();
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
    return displayGridColumns >= 8 ? 72 : 96;
}

int PadGridComponent::cellHeight() const
{
    return 68;
}

void PadGridComponent::updateCanvasSize()
{
    int maxDisplayRow = 0;
    for (const auto& pad : currentProfile.getPads())
        maxDisplayRow = juce::jmax (maxDisplayRow, pad.gridRow + (pad.gridCol / displayGridColumns));

    padCanvas.setSize (juce::jmax (displayGridColumns * cellWidth() + 16, viewport.getWidth()),
                       (maxDisplayRow + 1) * cellHeight() + 16);
}

juce::Rectangle<int> PadGridComponent::padBoundsForIndex (int index) const
{
    const auto& pads = currentProfile.getPads();
    if (index < 0 || index >= static_cast<int> (pads.size()))
        return {};

    const auto& pad = pads[static_cast<size_t> (index)];
    const int displayCol = pad.gridCol % displayGridColumns;
    const int displayRow = pad.gridRow + (pad.gridCol / displayGridColumns);

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
    auto buttonRow = area.removeFromTop (26);
    addPadButton.setBounds (buttonRow.removeFromLeft (buttonRow.getWidth() / 2).reduced (1));
    deletePadButton.setBounds (buttonRow.reduced (1));
    viewport.setBounds (area);
    updateCanvasSize();
    svc::ui::updatePadGridScrollbars (viewport, padCanvas);
}

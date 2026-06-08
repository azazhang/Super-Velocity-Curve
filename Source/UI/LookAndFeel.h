#pragma once

#include "Theme.h"
#include <JuceHeader.h>

namespace svc::ui
{

class AppLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AppLookAndFeel()
    {
        setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (Theme::background));
        setColour (juce::ComboBox::backgroundColourId, juce::Colour (Theme::panelRaised));
        setColour (juce::ComboBox::textColourId, juce::Colour (Theme::textPrimary));
        setColour (juce::ComboBox::outlineColourId, juce::Colour (Theme::border));
        setColour (juce::PopupMenu::backgroundColourId, juce::Colour (Theme::panelRaised));
        setColour (juce::PopupMenu::textColourId, juce::Colour (Theme::textPrimary));
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (Theme::accentDim));
        setColour (juce::TextEditor::backgroundColourId, juce::Colour (Theme::panelRaised));
        setColour (juce::TextEditor::textColourId, juce::Colour (Theme::textPrimary));
        setColour (juce::TextEditor::outlineColourId, juce::Colour (Theme::border));
        setColour (juce::ToggleButton::textColourId, juce::Colour (Theme::textPrimary));
        setColour (juce::Slider::thumbColourId, juce::Colour (Theme::accent));
        setColour (juce::Slider::trackColourId, juce::Colour (Theme::border));
        setColour (juce::Label::textColourId, juce::Colour (Theme::textPrimary));
    }

    void drawButtonBackground (juce::Graphics& g,
                               juce::Button& button,
                               const juce::Colour&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
        const auto base = shouldDrawButtonAsDown ? juce::Colour (Theme::accentDim)
                        : shouldDrawButtonAsHighlighted ? juce::Colour (Theme::padHover)
                        : juce::Colour (Theme::panelRaised);
        g.setColour (base);
        g.fillRoundedRectangle (bounds, 5.0f);
        g.setColour (juce::Colour (Theme::border));
        g.drawRoundedRectangle (bounds, 5.0f, 1.0f);
    }

    void drawButtonText (juce::Graphics& g,
                         juce::TextButton& button,
                         bool,
                         bool) override
    {
        g.setColour (button.isEnabled() ? juce::Colour (Theme::textPrimary)
                                        : juce::Colour (Theme::textSecondary));
        g.setFont (Theme::smallFont());
        g.drawFittedText (button.getButtonText(), button.getLocalBounds().reduced (4, 0),
                          juce::Justification::centred, 1);
    }
};

} // namespace svc::ui

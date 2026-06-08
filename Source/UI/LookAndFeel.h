#pragma once

#include "Theme.h"
#include <JuceHeader.h>

namespace svc::ui
{

class AppLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AppLookAndFeel() { applyThemeColours(); }

    void refreshTheme() { applyThemeColours(); }

    void drawButtonBackground (juce::Graphics& g,
                               juce::Button& button,
                               const juce::Colour&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
        const bool toggled = button.getToggleState();

        juce::Colour base = juce::Colour (Theme::panelRaised());
        if (toggled)
            base = juce::Colour (Theme::accentDim());
        else if (shouldDrawButtonAsDown)
            base = base.darker (0.15f);
        else if (shouldDrawButtonAsHighlighted)
            base = juce::Colour (Theme::padHover());

        g.setColour (base);
        g.fillRoundedRectangle (bounds, 5.0f);

        if (toggled)
        {
            g.setColour (juce::Colour (Theme::accentGold()).withAlpha (0.5f));
            g.drawRoundedRectangle (bounds.expanded (0.5f), 5.5f, 1.0f);
        }

        g.setColour (juce::Colour (Theme::border()).withAlpha (0.8f));
        g.drawRoundedRectangle (bounds, 5.0f, 1.0f);
    }

    void drawButtonText (juce::Graphics& g,
                         juce::TextButton& button,
                         bool,
                         bool) override
    {
        g.setColour (button.isEnabled() ? juce::Colour (Theme::textPrimary())
                                        : juce::Colour (Theme::textSecondary()));
        g.setFont (Theme::smallFont());
        g.drawFittedText (button.getButtonText(), button.getLocalBounds().reduced (4, 0),
                          juce::Justification::centred, 1);
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool,
                       int, int, int, int, juce::ComboBox& box) override
    {
        auto bounds = juce::Rectangle<float> (0.0f, 0.0f, static_cast<float> (width), static_cast<float> (height)).reduced (0.5f);
        Theme::fillPanel (g, bounds, 5.0f);

        if (box.hasKeyboardFocus (false))
        {
            g.setColour (juce::Colour (Theme::accent()).withAlpha (0.35f));
            g.drawRoundedRectangle (bounds.expanded (0.5f), 5.5f, 1.0f);
        }
    }

private:
    void applyThemeColours()
    {
        setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (Theme::background()));
        setColour (juce::ComboBox::backgroundColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::ComboBox::textColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::ComboBox::outlineColourId, juce::Colour (Theme::border()));
        setColour (juce::ComboBox::arrowColourId, juce::Colour (Theme::accentGold()));
        setColour (juce::PopupMenu::backgroundColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::PopupMenu::textColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (Theme::accentDim()));
        setColour (juce::TextEditor::backgroundColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::TextEditor::textColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::TextEditor::outlineColourId, juce::Colour (Theme::border()));
        setColour (juce::TextEditor::focusedOutlineColourId, juce::Colour (Theme::accent()).withAlpha (0.55f));
        setColour (juce::ToggleButton::textColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::ToggleButton::tickColourId, juce::Colour (Theme::accentGold()));
        setColour (juce::Slider::thumbColourId, juce::Colour (Theme::accentGold()));
        setColour (juce::Slider::trackColourId, juce::Colour (Theme::border()));
        setColour (juce::Slider::backgroundColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::Label::textColourId, juce::Colour (Theme::textPrimary()));
    }
};

} // namespace svc::ui

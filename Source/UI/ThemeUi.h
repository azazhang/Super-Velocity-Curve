#pragma once

#include "Theme.h"
#include <JuceHeader.h>

namespace svc::ui
{

inline void applyTextEditorTheme (juce::TextEditor& editor)
{
    const auto text = juce::Colour (Theme::textPrimary());
    const auto bg = juce::Colour (Theme::panelRaised());
    const auto border = juce::Colour (Theme::border());

    editor.setFont (Theme::bodyFont());
    editor.setColour (juce::TextEditor::textColourId, text);
    editor.setColour (juce::TextEditor::backgroundColourId, bg);
    editor.setColour (juce::TextEditor::outlineColourId, border);
    editor.setColour (juce::TextEditor::focusedOutlineColourId,
                      juce::Colour (Theme::accent()).withAlpha (0.55f));
    editor.setColour (juce::CaretComponent::caretColourId, juce::Colour (Theme::accentGold()));
    editor.setColour (juce::TextEditor::highlightColourId, juce::Colour (Theme::accentDim()));
    editor.setColour (juce::TextEditor::highlightedTextColourId, text);
    editor.applyColourToAllText (text, false);
}

inline void applySliderTheme (juce::Slider& slider)
{
    const auto text = juce::Colour (Theme::textPrimary());
    const auto bg = juce::Colour (Theme::panelRaised());
    const auto border = juce::Colour (Theme::border());

    slider.setColour (juce::Slider::thumbColourId, juce::Colour (Theme::accentGold()));
    slider.setColour (juce::Slider::trackColourId, juce::Colour (Theme::accent()).withAlpha (0.55f));
    slider.setColour (juce::Slider::backgroundColourId, juce::Colour (Theme::border()).withAlpha (0.35f));
    slider.setColour (juce::Slider::textBoxTextColourId, text);
    slider.setColour (juce::Slider::textBoxBackgroundColourId, bg);
    slider.setColour (juce::Slider::textBoxOutlineColourId, border);
    slider.setColour (juce::Slider::textBoxHighlightColourId, juce::Colour (Theme::accent()).withAlpha (0.25f));

    for (auto* child : slider.getChildren())
    {
        if (auto* label = dynamic_cast<juce::Label*> (child))
        {
            label->setColour (juce::Label::textColourId, text);
            label->setColour (juce::Label::backgroundColourId, bg);
            label->setColour (juce::Label::outlineColourId, border);
            label->setColour (juce::TextEditor::textColourId, text);
            label->setColour (juce::TextEditor::backgroundColourId, bg);
            label->setColour (juce::TextEditor::outlineColourId, border);
            label->setColour (juce::TextEditor::highlightColourId, juce::Colour (Theme::accent()).withAlpha (0.25f));
        }
    }
}

inline void applyToggleTheme (juce::ToggleButton& toggle)
{
    toggle.setColour (juce::ToggleButton::textColourId, juce::Colour (Theme::textPrimary()));
    toggle.setColour (juce::ToggleButton::tickColourId, juce::Colour (Theme::accentGold()));
    toggle.setColour (juce::ToggleButton::tickDisabledColourId, juce::Colour (Theme::border()));
}

inline void applyTextButtonTheme (juce::TextButton& button)
{
    button.setColour (juce::TextButton::buttonColourId, juce::Colour (Theme::panelRaised()));
    button.setColour (juce::TextButton::buttonOnColourId, juce::Colour (Theme::accentDim()));
    button.setColour (juce::TextButton::textColourOffId, juce::Colour (Theme::textPrimary()));
    button.setColour (juce::TextButton::textColourOnId, juce::Colour (Theme::textPrimary()));
}

inline void applyComboBoxTheme (juce::ComboBox& box)
{
    const auto text = juce::Colour (Theme::textPrimary());
    const auto bg = juce::Colour (Theme::panelRaised());
    const auto border = juce::Colour (Theme::border());

    box.setColour (juce::ComboBox::textColourId, text);
    box.setColour (juce::ComboBox::backgroundColourId, bg);
    box.setColour (juce::ComboBox::outlineColourId, border);
    box.setColour (juce::ComboBox::arrowColourId, juce::Colour (Theme::accentGold()));
}

} // namespace svc::ui

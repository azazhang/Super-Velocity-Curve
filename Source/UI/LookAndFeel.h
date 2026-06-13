#pragma once

#include "Theme.h"
#include <JuceHeader.h>

namespace svc::ui
{

class AppLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AppLookAndFeel() { refreshTheme(); }

    void refreshTheme()
    {
        setColourScheme (svc::ui::Theme::getMode() == svc::ui::ThemeMode::light
                             ? getLightColourScheme()
                             : getDarkColourScheme());
        applyThemeColours();
    }

    void drawButtonBackground (juce::Graphics& g,
                               juce::Button& button,
                               const juce::Colour&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced (1.0f, 0.5f);
        const bool toggled = button.getToggleState();

        juce::Colour base = juce::Colour (Theme::panelRaised());
        if (toggled)
            base = juce::Colour (Theme::accentDim());
        else if (shouldDrawButtonAsDown)
            base = base.darker (0.15f);
        else if (shouldDrawButtonAsHighlighted)
            base = juce::Colour (Theme::padHover());

        g.setColour (base);
        g.fillRoundedRectangle (bounds, 6.0f);

        if (toggled)
        {
            g.setColour (juce::Colour (Theme::accentGold()).withAlpha (0.5f));
            g.drawRoundedRectangle (bounds.expanded (0.5f), 5.5f, 1.0f);
        }

        g.setColour (juce::Colour (Theme::border()).withAlpha (0.8f));
        g.drawRoundedRectangle (bounds, 6.0f, 1.0f);
    }

    void drawTabButton (juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        const auto area = button.getLocalBounds().toFloat().reduced (1.0f, 2.0f);
        const bool active = button.isFrontTab();

        g.setColour (active ? juce::Colour (Theme::panelRaised())
                            : juce::Colour (Theme::panel()).withAlpha (0.6f));
        g.fillRoundedRectangle (area, 5.0f);

        if (active)
        {
            g.setColour (juce::Colour (Theme::accentGold()).withAlpha (0.45f));
            g.fillRect (area.getX(), area.getBottom() - 2.0f, area.getWidth(), 2.0f);
        }

        g.setColour (juce::Colour (Theme::textPrimary()).withAlpha (active ? 1.0f : 0.75f));
        g.setFont (Theme::smallFont().boldened());
        g.drawFittedText (button.getButtonText(), area.toNearestInt(), juce::Justification::centred, 1);
        juce::ignoreUnused (isMouseOver, isMouseDown);
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

        const auto arrowZone = bounds.removeFromRight (18.0f).reduced (4.0f, 6.0f);
        juce::Path arrow;
        arrow.addTriangle (arrowZone.getCentreX() - 4.0f, arrowZone.getCentreY() - 2.0f,
                           arrowZone.getCentreX() + 4.0f, arrowZone.getCentreY() - 2.0f,
                           arrowZone.getCentreX(), arrowZone.getCentreY() + 3.0f);
        g.setColour (box.findColour (juce::ComboBox::arrowColourId));
        g.fillPath (arrow);
    }

    juce::Label* createComboBoxTextBox (juce::ComboBox& box) override
    {
        auto* label = LookAndFeel_V4::createComboBoxTextBox (box);
        label->setColour (juce::Label::textColourId, box.findColour (juce::ComboBox::textColourId));
        label->setFont (Theme::bodyFont());
        return label;
    }

    void fillTextEditorBackground (juce::Graphics& g, int width, int height, juce::TextEditor& editor) override
    {
        g.setColour (editor.findColour (juce::TextEditor::backgroundColourId));
        g.fillRoundedRectangle (1.0f, 1.0f, static_cast<float> (width) - 2.0f, static_cast<float> (height) - 2.0f, 5.0f);
        g.setColour (editor.findColour (juce::TextEditor::outlineColourId));
        g.drawRoundedRectangle (0.5f, 0.5f, static_cast<float> (width) - 1.0f, static_cast<float> (height) - 1.0f, 5.0f, 1.0f);
    }

    void drawLabel (juce::Graphics& g, juce::Label& label) override
    {
        if (label.isBeingEdited())
        {
            if (auto* editor = label.getCurrentTextEditor())
            {
                editor->setBounds (label.getLocalBounds());
                editor->setVisible (true);
            }
            return;
        }

        const auto bg = label.findColour (juce::Label::backgroundColourId);
        if (bg.isOpaque())
            g.fillAll (bg);

        const auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (label.getFont());
        g.drawFittedText (label.getText(), label.getLocalBounds().reduced (1),
                          label.getJustificationType(), juce::jmax (1, (int) (label.getHeight() / label.getFont().getHeight())),
                          label.getMinimumHorizontalScale());
    }

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::ignoreUnused (shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        auto bounds = button.getLocalBounds().toFloat().reduced (1.0f);
        const auto tickSize = juce::jmin (18.0f, bounds.getHeight() - 2.0f);
        auto tick = bounds.removeFromLeft (tickSize).withSizeKeepingCentre (tickSize, tickSize);

        g.setColour (juce::Colour (Theme::panelRaised()));
        g.fillRoundedRectangle (tick, 3.0f);
        g.setColour (juce::Colour (Theme::border()));
        g.drawRoundedRectangle (tick, 3.0f, 1.0f);

        if (button.getToggleState())
        {
            g.setColour (juce::Colour (Theme::accentGold()));
            g.fillRoundedRectangle (tick.reduced (4.0f), 2.0f);
        }

        g.setColour (button.findColour (juce::ToggleButton::textColourId));
        g.setFont (Theme::bodyFont());
        g.drawFittedText (button.getButtonText(), bounds.reduced (4.0f, 0.0f).toNearestInt(),
                          juce::Justification::centredLeft, 2);
    }

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        juce::ignoreUnused (minSliderPos, maxSliderPos);

        if (slider.isBar())
        {
            LookAndFeel_V4::drawLinearSlider (g, x, y, width, height, sliderPos,
                                              minSliderPos, maxSliderPos, style, slider);
            return;
        }

        const auto trackWidth = juce::jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);
        const auto trackBounds = juce::Rectangle<float> (static_cast<float> (x), static_cast<float> (y),
                                                       static_cast<float> (width), static_cast<float> (height));

        juce::Path track;
        if (slider.isHorizontal())
        {
            const auto yMid = trackBounds.getCentreY();
            track.startNewSubPath (trackBounds.getX(), yMid);
            track.lineTo (trackBounds.getRight(), yMid);
        }
        else
        {
            const auto xMid = trackBounds.getCentreX();
            track.startNewSubPath (xMid, trackBounds.getBottom());
            track.lineTo (xMid, trackBounds.getY());
        }

        g.setColour (slider.findColour (juce::Slider::backgroundColourId));
        g.strokePath (track, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        juce::Point<float> thumbPoint;
        if (slider.isHorizontal())
            thumbPoint = { sliderPos, trackBounds.getCentreY() };
        else
            thumbPoint = { trackBounds.getCentreX(), sliderPos };

        juce::Path valueTrack;
        if (slider.isHorizontal())
        {
            valueTrack.startNewSubPath (trackBounds.getX(), trackBounds.getCentreY());
            valueTrack.lineTo (thumbPoint.x, thumbPoint.y);
        }
        else
        {
            valueTrack.startNewSubPath (trackBounds.getCentreX(), trackBounds.getBottom());
            valueTrack.lineTo (thumbPoint.x, thumbPoint.y);
        }

        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        const auto thumbSize = juce::jmin (14.0f, slider.isHorizontal() ? (float) height * 0.72f : (float) width * 0.72f);
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        g.fillEllipse (juce::Rectangle<float> (thumbSize, thumbSize).withCentre (thumbPoint));
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
        setColour (juce::TextButton::buttonColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::TextButton::buttonOnColourId, juce::Colour (Theme::accentDim()));
        setColour (juce::TextButton::textColourOffId, juce::Colour (Theme::textPrimary()));
        setColour (juce::TextButton::textColourOnId, juce::Colour (Theme::textPrimary()));
        setColour (juce::ToggleButton::textColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::ToggleButton::tickColourId, juce::Colour (Theme::accentGold()));
        setColour (juce::ToggleButton::tickDisabledColourId, juce::Colour (Theme::border()));
        setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::thumbColourId, juce::Colour (Theme::accentGold()));
        setColour (juce::Slider::trackColourId, juce::Colour (Theme::border()));
        setColour (juce::Slider::backgroundColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::Slider::textBoxTextColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (Theme::panelRaised()));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (Theme::border()));
        setColour (juce::Slider::textBoxHighlightColourId, juce::Colour (Theme::accent()).withAlpha (0.25f));
        setColour (juce::Label::textColourId, juce::Colour (Theme::textPrimary()));
        setColour (juce::ScrollBar::backgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::ScrollBar::thumbColourId, juce::Colour (Theme::border()).withAlpha (0.55f));
        setColour (juce::ScrollBar::trackColourId, juce::Colours::transparentBlack);
    }

    void drawScrollbar (juce::Graphics& g,
                        juce::ScrollBar& scrollbar,
                        int x,
                        int y,
                        int width,
                        int height,
                        bool isScrollbarVertical,
                        int thumbStartPosition,
                        int thumbSize,
                        bool isMouseOver,
                        bool isMouseDown) override
    {
        juce::ignoreUnused (scrollbar, isMouseOver, isMouseDown);

        if (thumbSize <= 0)
            return;

        const int trackLength = isScrollbarVertical ? height : width;
        if (thumbSize >= trackLength - 2)
            return;

        g.setColour (juce::Colour (Theme::border()).withAlpha (0.45f));
        const int thickness = juce::jmin (isScrollbarVertical ? width : height, 6);

        if (isScrollbarVertical)
            g.fillRoundedRectangle (static_cast<float> (x + (width - thickness) / 2),
                                    static_cast<float> (thumbStartPosition),
                                    static_cast<float> (thickness),
                                    static_cast<float> (thumbSize),
                                    3.0f);
        else
            g.fillRoundedRectangle (static_cast<float> (thumbStartPosition),
                                    static_cast<float> (y + (height - thickness) / 2),
                                    static_cast<float> (thumbSize),
                                    static_cast<float> (thickness),
                                    3.0f);
    }
};

} // namespace svc::ui

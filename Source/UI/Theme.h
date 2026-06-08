#pragma once

#include <JuceHeader.h>

namespace svc::ui
{

enum class ThemeMode
{
    dark,
    light
};

/** Logic-inspired palette — dark default; light for bright DAW themes. */
struct Theme
{
    static void setMode (ThemeMode mode) noexcept { currentMode = mode; }
    static ThemeMode getMode() noexcept { return currentMode; }

    static juce::uint32 background()       { return currentMode == ThemeMode::dark ? 0xff16161a : 0xfff0f1f4; }
    static juce::uint32 backgroundTop()    { return currentMode == ThemeMode::dark ? 0xff1c1c22 : 0xfff8f9fb; }
    static juce::uint32 panel()            { return currentMode == ThemeMode::dark ? 0xff24242c : 0xffffffff; }
    static juce::uint32 panelRaised()      { return currentMode == ThemeMode::dark ? 0xff2c2c36 : 0xfff4f5f7; }
    static juce::uint32 border()           { return currentMode == ThemeMode::dark ? 0xff3a3a48 : 0xffd1d5db; }
    static juce::uint32 borderBright()     { return currentMode == ThemeMode::dark ? 0xff4a4a58 : 0xffb8bcc4; }
    static juce::uint32 accent()           { return currentMode == ThemeMode::dark ? 0xff5eb0ff : 0xff2563eb; }
    static juce::uint32 accentGold()       { return currentMode == ThemeMode::dark ? 0xffd4a84b : 0xffb8860b; }
    static juce::uint32 accentSecondary()  { return currentMode == ThemeMode::dark ? 0xffc77dff : 0xff7c3aed; }
    static juce::uint32 accentDim()        { return currentMode == ThemeMode::dark ? 0xff2a4a66 : 0xffbfdbfe; }
    static juce::uint32 accentWarm()       { return currentMode == ThemeMode::dark ? 0xffe8b84a : 0xffd97706; }
    static juce::uint32 textPrimary()      { return currentMode == ThemeMode::dark ? 0xfff0f0f4 : 0xff111827; }
    static juce::uint32 textSecondary()    { return currentMode == ThemeMode::dark ? 0xff9b9ba8 : 0xff6b7280; }
    static juce::uint32 padIdle()          { return currentMode == ThemeMode::dark ? 0xff2a2a34 : 0xffe5e7eb; }
    static juce::uint32 padHover()         { return currentMode == ThemeMode::dark ? 0xff343440 : 0xffd1d5db; }
    static juce::uint32 padSelected()      { return currentMode == ThemeMode::dark ? 0xff3d4f66 : 0xff93c5fd; }
    static juce::uint32 padHit()           { return currentMode == ThemeMode::dark ? 0xffe8b84a : 0xfff59e0b; }
    static juce::uint32 padDisabled()      { return currentMode == ThemeMode::dark ? 0xff1a1a20 : 0xfff3f4f6; }
    static juce::uint32 curveLine()        { return currentMode == ThemeMode::dark ? 0xff5eb0ff : 0xff2563eb; }
    static juce::uint32 curveLineEnd()     { return currentMode == ThemeMode::dark ? 0xff8ec5ff : 0xff60a5fa; }
    static juce::uint32 curveGrid()        { return currentMode == ThemeMode::dark ? 0xff32323e : 0xffe5e7eb; }
    static juce::uint32 curveHit()         { return currentMode == ThemeMode::dark ? 0xffd4a84b : 0xffb8860b; }
    static juce::uint32 curveGateMuted()   { return currentMode == ThemeMode::dark ? 0xff1a1520 : 0xfffee2e2; }
    static juce::uint32 curveGateSaturated() { return currentMode == ThemeMode::dark ? 0xff101828 : 0xffdbeafe; }
    static juce::uint32 plotBackground()   { return currentMode == ThemeMode::dark ? 0xff121218 : 0xffe8eaed; }
    static juce::uint32 success()          { return currentMode == ThemeMode::dark ? 0xff4ade80 : 0xff16a34a; }

    static juce::Font titleFont()       { return juce::Font (juce::FontOptions (20.0f)).boldened(); }
    static juce::Font sectionFont()     { return juce::Font (juce::FontOptions (11.5f)).boldened(); }
    static juce::Font bodyFont()        { return juce::Font (juce::FontOptions (12.0f)); }
    static juce::Font smallFont()       { return juce::Font (juce::FontOptions (10.5f)); }
    static juce::Font sectionHeaderFont() { return juce::Font (juce::FontOptions (10.0f)).boldened(); }

    static void fillBackgroundGradient (juce::Graphics& g, juce::Rectangle<int> bounds)
    {
        juce::ColourGradient grad (juce::Colour (backgroundTop()), 0.0f, 0.0f,
                                   juce::Colour (background()), 0.0f, static_cast<float> (bounds.getHeight()),
                                   false);
        g.setGradientFill (grad);
        g.fillRect (bounds);
    }

    static void fillPanel (juce::Graphics& g, juce::Rectangle<float> bounds, float radius = 8.0f)
    {
        g.setColour (juce::Colour (panel()));
        g.fillRoundedRectangle (bounds, radius);
        g.setColour (juce::Colour (border()).withAlpha (0.7f));
        g.drawRoundedRectangle (bounds, radius, 1.0f);
    }

    static void fillPlot (juce::Graphics& g, juce::Rectangle<float> plot, float radius = 6.0f)
    {
        g.setColour (juce::Colour (plotBackground()));
        g.fillRoundedRectangle (plot, radius);
        g.setColour (juce::Colour (border()).withAlpha (0.5f));
        g.drawRoundedRectangle (plot, radius, 1.0f);
    }

    static void drawSectionHeader (juce::Graphics& g, juce::Rectangle<int> area, const juce::String& text)
    {
        g.setFont (sectionHeaderFont());
        g.setColour (juce::Colour (textSecondary()));
        g.drawText (text.toUpperCase(), area, juce::Justification::centredLeft);
    }

private:
    static inline ThemeMode currentMode = ThemeMode::dark;
};

} // namespace svc::ui

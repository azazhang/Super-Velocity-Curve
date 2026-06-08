#pragma once

#include "Theme.h"
#include <JuceHeader.h>
#include <functional>

class AboutPanelComponent : public juce::Component
{
public:
    AboutPanelComponent();

    std::function<void()> onDismiss;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& event) override;

private:
    juce::Rectangle<int> cardBounds;

    juce::Label titleLabel;
    juce::Label versionLabel;
    juce::Label blurbLabel;
    juce::TextButton closeButton { "Close" };
    juce::TextButton githubButton { "GitHub" };
    juce::TextButton homeButton { "Homepage" };
    juce::TextButton youtubeButton { "YouTube" };
    juce::TextButton supportButton { "Buy me a coffee" };
    juce::ImageButton pluginvalBadgeButton;

    void openUrl (const juce::String& url) const;
};

#include "AboutPanelComponent.h"
#include "../Config/AppBranding.h"
#include "../Config/AppUrls.h"
#include <BinaryData.h>

AboutPanelComponent::AboutPanelComponent()
{
    titleLabel.setText (svc::branding::kProductName, juce::dontSendNotification);
    titleLabel.setFont (svc::ui::Theme::titleFont());
    titleLabel.setJustificationType (juce::Justification::centred);

   #ifdef SVC_VERSION
    versionLabel.setText ("Version " SVC_VERSION, juce::dontSendNotification);
   #else
    versionLabel.setText ("Version 0.6.0", juce::dontSendNotification);
   #endif
    versionLabel.setFont (svc::ui::Theme::bodyFont());
    versionLabel.setColour (juce::Label::textColourId, juce::Colour (svc::ui::Theme::textSecondary()));
    versionLabel.setJustificationType (juce::Justification::centred);

    developerKickerLabel.setText ("Developer", juce::dontSendNotification);
    developerKickerLabel.setFont (svc::ui::Theme::smallFont());
    developerKickerLabel.setColour (juce::Label::textColourId, juce::Colour (svc::ui::Theme::textSecondary()));
    developerKickerLabel.setJustificationType (juce::Justification::centredLeft);

    developerNameLabel.setText ("azazhang / Studio J", juce::dontSendNotification);
    developerNameLabel.setFont (svc::ui::Theme::bodyFont().withHeight (17.0f));
    developerNameLabel.setJustificationType (juce::Justification::centredLeft);

    blurbLabel.setText ("Per-pad MIDI velocity curves — finger drumming, e-kits, and any setup\n"
                        "where one global DAW curve is not enough. pluginval strictness 5 (tap badge).",
                        juce::dontSendNotification);
    blurbLabel.setFont (svc::ui::Theme::smallFont());
    blurbLabel.setJustificationType (juce::Justification::centred);

    portraitImage = juce::ImageCache::getFromMemory (BinaryData::developer_portrait_png,
                                                     BinaryData::developer_portrait_pngSize);

    const auto badge = juce::ImageCache::getFromMemory (BinaryData::pluginval_badge_png,
                                                        BinaryData::pluginval_badge_pngSize);
    pluginvalBadgeButton.setImages (false, true, true, badge, 1.0f, juce::Colours::transparentBlack,
                                    badge, 1.0f, juce::Colours::transparentBlack,
                                    badge, 1.0f, juce::Colour (svc::ui::Theme::accent()).withAlpha (0.25f));
    pluginvalBadgeButton.setTooltip ("Verified by pluginval");
    pluginvalBadgeButton.onClick = [this] { openUrl (svc::urls::kPluginvalInfo); };

    githubButton.onClick = [this] { openUrl (svc::urls::kGitHubRepo); };
    homeButton.onClick = [this] { openUrl (svc::urls::kHomepage); };
    homeButton.setTooltip ("azhang.eu.org");
    youtubeButton.onClick = [this] { openUrl (svc::urls::kYouTube); };
    youtubeButton.setTooltip ("Studio J on YouTube");
    supportButton.onClick = [this] { openUrl (svc::urls::kSupport); };

    closeButton.onClick = [this]
    {
        if (onDismiss)
            onDismiss();
    };

    for (auto* c : { &titleLabel, &versionLabel, &developerKickerLabel, &developerNameLabel, &blurbLabel })
        addAndMakeVisible (c);

    for (auto* b : { &closeButton, &githubButton, &homeButton, &youtubeButton, &supportButton })
    {
        addAndMakeVisible (b);
        b->setMouseClickGrabsKeyboardFocus (false);
    }

    addAndMakeVisible (pluginvalBadgeButton);
    pluginvalBadgeButton.setMouseClickGrabsKeyboardFocus (false);
}

void AboutPanelComponent::openUrl (const juce::String& url) const
{
    juce::URL (url).launchInDefaultBrowser();
}

void AboutPanelComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.55f));
    svc::ui::Theme::fillPanel (g, cardBounds.toFloat(), 12.0f);

    if (portraitImage.isValid() && ! portraitBounds.isEmpty())
    {
        juce::Path circle;
        circle.addEllipse (portraitBounds.toFloat());

        g.saveState();
        g.reduceClipRegion (circle);
        g.drawImage (portraitImage, portraitBounds.toFloat(),
                     juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize);
        g.restoreState();

        g.setColour (juce::Colour (svc::ui::Theme::border()).withAlpha (0.85f));
        g.strokePath (circle, juce::PathStrokeType (2.0f));
    }
}

void AboutPanelComponent::resized()
{
    cardBounds = getLocalBounds().withSizeKeepingCentre (480, 448);
    auto content = cardBounds.reduced (24, 20);

    titleLabel.setBounds (content.removeFromTop (28));
    versionLabel.setBounds (content.removeFromTop (20));
    content.removeFromTop (10);

    auto developerRow = content.removeFromTop (88);
    portraitBounds = developerRow.removeFromLeft (88);
    developerRow.removeFromLeft (14);
    developerKickerLabel.setBounds (developerRow.removeFromTop (18));
    developerNameLabel.setBounds (developerRow.removeFromTop (24));

    content.removeFromTop (10);
    blurbLabel.setBounds (content.removeFromTop (44));
    content.removeFromTop (12);

    auto row1 = content.removeFromTop (28);
    githubButton.setBounds (row1.removeFromLeft (row1.getWidth() / 2).reduced (2));
    homeButton.setBounds (row1.reduced (2));

    auto row2 = content.removeFromTop (28);
    youtubeButton.setBounds (row2.removeFromLeft (row2.getWidth() / 2).reduced (2));
    supportButton.setBounds (row2.reduced (2));

    content.removeFromTop (8);
    pluginvalBadgeButton.setBounds (content.removeFromTop (64).reduced (72, 0));
    closeButton.setBounds (content.removeFromBottom (28).reduced (80, 0));
}

void AboutPanelComponent::mouseDown (const juce::MouseEvent& event)
{
    if (! cardBounds.contains (event.getPosition()) && onDismiss)
        onDismiss();
}

#include "UnsavedProfileDialogComponent.h"

UnsavedProfileDialogComponent::UnsavedProfileDialogComponent()
{
    titleLabel.setText ("Unsaved profile changes", juce::dontSendNotification);
    titleLabel.setFont (svc::ui::Theme::sectionFont());
    titleLabel.setJustificationType (juce::Justification::centredLeft);

    messageLabel.setText ("Save changes to the current profile before switching?",
                          juce::dontSendNotification);
    messageLabel.setFont (svc::ui::Theme::bodyFont());
    messageLabel.setJustificationType (juce::Justification::centredLeft);

    saveButton.onClick = [this] { finish (Choice::save); };
    discardButton.onClick = [this] { finish (Choice::discard); };
    cancelButton.onClick = [this] { finish (Choice::cancel); };

    for (auto* c : { &titleLabel, &messageLabel })
        addAndMakeVisible (c);

    for (auto* b : { &saveButton, &discardButton, &cancelButton })
    {
        addAndMakeVisible (b);
        b->setMouseClickGrabsKeyboardFocus (false);
    }
}

void UnsavedProfileDialogComponent::finish (Choice choice)
{
    if (onChoice)
        onChoice (choice);
}

void UnsavedProfileDialogComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.55f));
    svc::ui::Theme::fillPanel (g, cardBounds.toFloat(), 12.0f);
}

void UnsavedProfileDialogComponent::resized()
{
    cardBounds = getLocalBounds().withSizeKeepingCentre (400, 160);
    auto content = cardBounds.reduced (24, 20);

    titleLabel.setBounds (content.removeFromTop (24));
    content.removeFromTop (8);
    messageLabel.setBounds (content.removeFromTop (40));
    content.removeFromTop (12);

    auto row = content.removeFromTop (28);
    const int w = row.getWidth() / 3;
    saveButton.setBounds (row.removeFromLeft (w).reduced (2));
    discardButton.setBounds (row.removeFromLeft (w).reduced (2));
    cancelButton.setBounds (row.reduced (2));
}

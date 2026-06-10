#include "CollapsibleSection.h"



CollapsibleSection::CollapsibleSection (const juce::String& title,

                                        juce::Component& contentComponent,

                                        int expandedContentHeight,

                                        bool startExpanded)

    : sectionTitle (title),

      content (contentComponent),

      contentHeight (expandedContentHeight),

      expanded (startExpanded)

{

    headerButton.setClickingTogglesState (false);

    headerButton.onClick = [this]

    {

        setExpanded (! expanded);

        if (onLayoutChanged)

            onLayoutChanged();

    };



    addAndMakeVisible (headerButton);

    addAndMakeVisible (content);

    content.setVisible (expanded);

    refreshHeader();

}



void CollapsibleSection::setContentHeightLimits (int minHeight, int maxHeight) noexcept

{

    minContentHeight = minHeight;

    maxContentHeight = juce::jmax (minHeight, maxHeight);

    contentHeight = juce::jlimit (minContentHeight, maxContentHeight, contentHeight);

}



void CollapsibleSection::setContentHeight (int newHeight)

{

    const auto clamped = juce::jlimit (minContentHeight, maxContentHeight, newHeight);

    if (contentHeight == clamped)

        return;



    contentHeight = clamped;

    resized();

    repaint();



    if (onLayoutChanged)

        onLayoutChanged();

}



void CollapsibleSection::setExpanded (bool shouldExpand)

{

    if (expanded == shouldExpand)

        return;



    expanded = shouldExpand;

    content.setVisible (expanded);

    refreshHeader();

    resized();

    repaint();

    if (onExpandedChanged)
        onExpandedChanged (expanded);
}



int CollapsibleSection::getTotalHeight() const noexcept

{

    return kHeaderHeight + (expanded ? contentHeight : 0);

}



bool CollapsibleSection::isResizeHandle (juce::Point<int> pos) const noexcept

{

    return expanded && pos.y >= getHeight() - kResizeHandleHeight;

}



void CollapsibleSection::refreshHeader()

{

    headerButton.setButtonText ((expanded ? "v " : "> ") + sectionTitle);

}



void CollapsibleSection::paint (juce::Graphics& g)

{

    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 6.0f);



    if (! expanded)

        return;



    const auto handle = getLocalBounds().removeFromBottom (kResizeHandleHeight).toFloat().reduced (8.0f, 2.0f);

    g.setColour (juce::Colour (svc::ui::Theme::textSecondary()).withAlpha (0.45f));

    const auto midY = handle.getCentreY();

    for (int i = 0; i < 3; ++i)

    {

        const auto x = handle.getX() + static_cast<float> (i) * (handle.getWidth() / 2.0f);

        g.fillEllipse (x - 2.0f, midY - 1.5f, 4.0f, 3.0f);

    }

}



void CollapsibleSection::resized()

{

    auto area = getLocalBounds().reduced (2);

    headerButton.setBounds (area.removeFromTop (kHeaderHeight));



    if (expanded)

        content.setBounds (area.withTrimmedBottom (kResizeHandleHeight));

}



void CollapsibleSection::mouseDown (const juce::MouseEvent& event)

{

    if (! isResizeHandle (event.getPosition()))

        return;



    resizeDragActive = true;

    resizeDragStartY = event.getPosition().y;

    resizeDragStartHeight = contentHeight;

}



void CollapsibleSection::mouseDrag (const juce::MouseEvent& event)

{

    if (! resizeDragActive)

        return;



    const auto delta = event.getPosition().y - resizeDragStartY;

    setContentHeight (resizeDragStartHeight + delta);

}



void CollapsibleSection::mouseUp (const juce::MouseEvent&)

{

    resizeDragActive = false;

}



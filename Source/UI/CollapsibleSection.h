#pragma once



#include "Theme.h"

#include <JuceHeader.h>

#include <functional>



/** Header bar + content; collapsed shows header only. Bottom edge drag-resizes when expanded. */

class CollapsibleSection : public juce::Component

{

public:

    static constexpr int kHeaderHeight = 26;

    static constexpr int kResizeHandleHeight = 8;



    CollapsibleSection (const juce::String& title,

                        juce::Component& contentComponent,

                        int expandedContentHeight,

                        bool startExpanded = false);



    void setExpanded (bool shouldExpand);

    bool isExpanded() const noexcept { return expanded; }

    int getTotalHeight() const noexcept;

    int getContentHeight() const noexcept { return contentHeight; }



    void setContentHeight (int newHeight);

    void setContentHeightLimits (int minHeight, int maxHeight) noexcept;



    std::function<void()> onLayoutChanged;
    std::function<void (bool expanded)> onExpandedChanged;



    void paint (juce::Graphics& g) override;

    void resized() override;

    void mouseDown (const juce::MouseEvent& event) override;

    void mouseDrag (const juce::MouseEvent& event) override;

    void mouseUp (const juce::MouseEvent& event) override;



private:

    juce::String sectionTitle;

    juce::TextButton headerButton;

    juce::Component& content;

    int contentHeight = 0;

    int minContentHeight = 48;

    int maxContentHeight = 360;

    bool expanded = false;

    bool resizeDragActive = false;

    int resizeDragStartY = 0;

    int resizeDragStartHeight = 0;



    bool isResizeHandle (juce::Point<int> pos) const noexcept;

    void refreshHeader();

};



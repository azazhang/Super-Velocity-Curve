#pragma once

#include "../Profiles/ControllerProfile.h"
#include "Theme.h"
#include <JuceHeader.h>
#include <functional>
#include <unordered_map>

class PadGridComponent : public juce::Component
{
public:
    PadGridComponent();

    void setProfile (const svc::ControllerProfile& profile, bool resetSelection = false);
    void updatePad (int index, const svc::ProfilePad& pad);
    void setSelectedPadIndex (int index);
    void scrollPadIntoView (int index);
    int getDisplayGridColumns() const noexcept { return displayGridColumns; }
    int getSelectedPadIndex() const noexcept { return selectedPadIndex; }
    int getPadCanvasWidth() const noexcept { return padCanvas.getWidth(); }
    int getViewportClientWidth() const noexcept { return viewport.getWidth(); }
    bool needsHorizontalScroll() const noexcept { return getPadCanvasWidth() > getViewportClientWidth(); }
    int getPadCanvasHeight() const noexcept { return padCanvas.getHeight(); }
    int getViewportClientHeight() const noexcept { return viewport.getHeight(); }
    bool needsVerticalScroll() const noexcept { return getPadCanvasHeight() > getViewportClientHeight(); }
    bool isVerticalScrollbarShown() const noexcept { return viewport.isVerticalScrollBarShown(); }

    void flashPadHit (int note, int channel, float outputVelocity);
    void decayHitVisuals();
    bool hasActiveHitVisuals() const noexcept;
    void refreshVisualCache();

    std::function<void (int padIndex)> onPadSelected;
    std::function<void()> onAddPadRequested;
    std::function<void()> onDeletePadRequested;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    class PadCanvas : public juce::Component
    {
    public:
        explicit PadCanvas (PadGridComponent& owner);
        void paint (juce::Graphics& g) override;
        void mouseDown (const juce::MouseEvent& event) override;
        void mouseMove (const juce::MouseEvent& event) override;

    private:
        PadGridComponent& owner;
    };

    svc::ControllerProfile currentProfile;
    int displayGridColumns = 4;
    int selectedPadIndex = 0;
    int hoveredPadIndex = -1;

    struct HitVisual
    {
        float intensity = 0.0f;
        double lastUpdateMs = 0.0;
    };

    std::unordered_map<int, HitVisual> hitByPadIndex;

    juce::TextButton addPadButton { "Add pad" };
    juce::TextButton deletePadButton { "Delete pad" };
    juce::Viewport viewport;
    PadCanvas padCanvas;

    int padIndexAt (juce::Point<int> pos) const;
    juce::Rectangle<int> padBoundsForIndex (int index) const;
    void updateCanvasSize();
    int cellWidth() const;
    int cellHeight() const;

    friend class PadCanvas;
};

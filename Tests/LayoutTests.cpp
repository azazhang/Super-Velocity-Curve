#include "../Source/Plugin/PluginEditor.h"
#include "../Source/Plugin/PluginProcessor.h"
#include "../Source/UI/CollapsibleSection.h"
#include "../Source/UI/CurveEditorComponent.h"
#include "../Source/UI/EditorLayout.h"
#include "../Source/UI/PadGridComponent.h"
#include "../Source/UI/MidiNoteNames.h"
#include "../Source/UI/PadUiMerge.h"
#include <iostream>

#define EXPECT_TRUE(cond) \
    do { \
        if (! (cond)) { \
            std::cerr << "FAIL: " #cond " at " << __LINE__ << '\n'; \
            return 1; \
        } \
    } while (false)

template <typename T>
static T* findChildComponent (juce::Component& root)
{
    if (auto* match = dynamic_cast<T*> (&root))
        return match;

    for (int i = 0; i < root.getNumChildComponents(); ++i)
    {
        if (auto* found = findChildComponent<T> (*root.getChildComponent (i)))
            return found;
    }

    return nullptr;
}

static int testMinWindowCurveVisible()
{
    svc::ui::layout::EditorLayoutInputs inputs;
    inputs.editorBounds = { 0, 0, svc::ui::layout::kMinEditorWidth, svc::ui::layout::kMinEditorHeight };
    inputs.hasStandaloneMidiPanel = true;
    inputs.padSettingsExpanded = true;
  {
    const auto layout = svc::ui::layout::computeEditorLayout (inputs);
    EXPECT_TRUE (layout.curveEditorBounds.getHeight() >= svc::ui::layout::kMinCurvePlotHeight);
    EXPECT_TRUE (layout.curveEditorBounds.getWidth() >= svc::ui::layout::kMinCurvePlotWidth);
  }

    inputs.hasStandaloneMidiPanel = false;
  {
    const auto layout = svc::ui::layout::computeEditorLayout (inputs);
    EXPECT_TRUE (layout.curveEditorBounds.getHeight() >= svc::ui::layout::kMinCurvePlotHeight);
  }

    return 0;
}

static int testDefaultWindowWithCollapsedBottomSections()
{
    svc::ui::layout::EditorLayoutInputs inputs;
    inputs.editorBounds = { 0, 0, 1280, 860 };
    inputs.hasStandaloneMidiPanel = true;
    inputs.padSettingsExpanded = true;
    inputs.bottomSections.histogram = CollapsibleSection::kHeaderHeight;
    inputs.bottomSections.midiTools = CollapsibleSection::kHeaderHeight;
    inputs.bottomSections.calibration = CollapsibleSection::kHeaderHeight;

    const auto layout = svc::ui::layout::computeEditorLayout (inputs);
    EXPECT_TRUE (layout.curveEditorBounds.getHeight() >= 200);
    EXPECT_TRUE (layout.curveEditorBounds.getWidth() >= svc::ui::layout::kMinCurvePlotWidth);
    return 0;
}

static int testSectionContentHeightClamped()
{
    juce::Component content;
    CollapsibleSection section { "Limits", content, 120, true };
    section.setContentHeightLimits (72, 200);
    section.setContentHeight (500);
    EXPECT_TRUE (section.getContentHeight() == 200);
    EXPECT_TRUE (section.getTotalHeight() == CollapsibleSection::kHeaderHeight + 200);
    return 0;
}

static int testCollapsibleSectionHeights()
{
    juce::Component content;
    CollapsibleSection collapsed { "Test", content, 120, false };
    EXPECT_TRUE (collapsed.getTotalHeight() == CollapsibleSection::kHeaderHeight);

    CollapsibleSection expanded { "Test", content, 120, true };
    EXPECT_TRUE (expanded.getTotalHeight() == CollapsibleSection::kHeaderHeight + 120);

    expanded.setContentHeight (200);
    EXPECT_TRUE (expanded.getTotalHeight() == CollapsibleSection::kHeaderHeight + 200);
    return 0;
}

static int testPadUiMergePreservesCurvePoints()
{
    svc::ProfilePad curvePad;
    curvePad.curve.setControlPoints ({ { 0.0f, 0.0f }, { 0.5f, 0.7f }, { 1.0f, 1.0f } });
    curvePad.curve.setFloor (0.1f);
    curvePad.curve.setCeiling (0.9f);

    svc::ProfilePad inspectorPad;
    inspectorPad.label = "Snare";
    inspectorPad.midiNote = 38;
    inspectorPad.midiChannel = 10;
    inspectorPad.curve.setFloor (0.2f);
    inspectorPad.curve.setCeiling (0.85f);

    const auto merged = svc::ui::mergePadFromCurveAndInspector (curvePad, inspectorPad);
    EXPECT_TRUE (merged.label == "Snare");
    EXPECT_TRUE (merged.midiNote == 38);
    EXPECT_TRUE (merged.curve.getFloor() == 0.2f);
    EXPECT_TRUE (merged.curve.getCeiling() == 0.85f);
    EXPECT_TRUE (merged.curve.getControlPoints().size() == curvePad.curve.getControlPoints().size());
    EXPECT_TRUE (std::abs (merged.curve.getControlPoints()[1].output - 0.7f) < 0.001f);
    return 0;
}

static int testLaunchpadGridHorizontalScroll()
{
    PadGridComponent grid;
    grid.setSize (250, 420);
    grid.setProfile (svc::ControllerProfile::createLaunchpadDrumRack());
    grid.resized();

    EXPECT_TRUE (grid.getDisplayGridColumns() == 8);
    EXPECT_TRUE (grid.getPadCanvasWidth() > grid.getViewportClientWidth());
    EXPECT_TRUE (grid.needsHorizontalScroll());
    return 0;
}

static int testLaunchpadGridVerticalScrollbarWithoutResize()
{
    PadGridComponent grid;
    grid.setSize (400, 320);
    grid.resized();
    grid.setProfile (svc::ControllerProfile::createLaunchpadDrumRack());

    EXPECT_TRUE (grid.needsVerticalScroll());
    EXPECT_TRUE (grid.isVerticalScrollbarShown());
    return 0;
}

static int testMidiNoteDisplayFormat()
{
    const auto text = svc::ui::formatMidiNote (36);
    EXPECT_TRUE (text.contains ("36"));
    EXPECT_TRUE (text.contains ("C"));

    const auto shortName = svc::ui::formatMidiNoteShort (36);
    EXPECT_TRUE (shortName.contains ("C"));
    EXPECT_TRUE (! shortName.contains ("("));
    return 0;
}

static int testInvalidStateDoesNotCrash()
{
    SuperVelocityCurveAudioProcessor processor;
    const char garbage[] = "not-valid-plugin-state";
    processor.setStateInformation (garbage, static_cast<int> (sizeof (garbage)));
    juce::MemoryBlock empty;
    processor.setStateInformation (empty.getData(), 0);
    return 0;
}

static int testProcessorStateRoundtrip()
{
    SuperVelocityCurveAudioProcessor processor;
    processor.prepareToPlay (48000.0, 512);

    juce::MemoryBlock saved;
    processor.getStateInformation (saved);
    EXPECT_TRUE (saved.getSize() > 0);

    SuperVelocityCurveAudioProcessor restored;
    restored.setStateInformation (saved.getData(), static_cast<int> (saved.getSize()));
    restored.prepareToPlay (48000.0, 512);

    juce::MemoryBlock roundTrip;
    restored.getStateInformation (roundTrip);
    EXPECT_TRUE (roundTrip.getSize() > 0);
    return 0;
}

static int testHeadlessEditorMinLayout()
{
    juce::ScopedJuceInitialiser_GUI gui;
    SuperVelocityCurveAudioProcessor processor;
    std::unique_ptr<SuperVelocityCurveAudioProcessorEditor> editor (
        dynamic_cast<SuperVelocityCurveAudioProcessorEditor*> (processor.createEditor()));
    EXPECT_TRUE (editor != nullptr);

    editor->setSize (svc::ui::layout::kMinEditorWidth, svc::ui::layout::kMinEditorHeight);
    editor->resized();

    auto* curve = findChildComponent<CurveEditorComponent> (*editor);
    EXPECT_TRUE (curve != nullptr);
    EXPECT_TRUE (curve->getHeight() >= svc::ui::layout::kMinCurvePlotHeight);
    EXPECT_TRUE (curve->getWidth() >= svc::ui::layout::kMinCurvePlotWidth);
    return 0;
}

static int testStateAfterEditorDestroyed()
{
    juce::ScopedJuceInitialiser_GUI gui;
    SuperVelocityCurveAudioProcessor processor;
    processor.prepareToPlay (48000.0, 512);
    {
        std::unique_ptr<SuperVelocityCurveAudioProcessorEditor> editor (
            dynamic_cast<SuperVelocityCurveAudioProcessorEditor*> (processor.createEditor()));
        EXPECT_TRUE (editor != nullptr);
    }

    juce::MemoryBlock saved;
    processor.getStateInformation (saved);
    processor.setStateInformation (saved.getData(), static_cast<int> (saved.getSize()));
    return 0;
}

static int testEditorStateRoundtrip()
{
    juce::ScopedJuceInitialiser_GUI gui;
    SuperVelocityCurveAudioProcessor processor;
    processor.prepareToPlay (48000.0, 512);
    std::unique_ptr<SuperVelocityCurveAudioProcessorEditor> editor (
        dynamic_cast<SuperVelocityCurveAudioProcessorEditor*> (processor.createEditor()));
    EXPECT_TRUE (editor != nullptr);

    juce::MemoryBlock saved;
    processor.getStateInformation (saved);
    processor.setStateInformation (saved.getData(), static_cast<int> (saved.getSize()));
    editor->resized();
    return 0;
}

int main()
{
    if (testMinWindowCurveVisible() != 0) return 1;
    if (testDefaultWindowWithCollapsedBottomSections() != 0) return 1;
    if (testSectionContentHeightClamped() != 0) return 1;
    if (testCollapsibleSectionHeights() != 0) return 1;
    if (testPadUiMergePreservesCurvePoints() != 0) return 1;
    if (testLaunchpadGridHorizontalScroll() != 0) return 1;
    if (testLaunchpadGridVerticalScrollbarWithoutResize() != 0) return 1;
    if (testMidiNoteDisplayFormat() != 0) return 1;
    if (testInvalidStateDoesNotCrash() != 0) return 1;
    if (testProcessorStateRoundtrip() != 0) return 1;
    if (testHeadlessEditorMinLayout() != 0) return 1;
    if (testStateAfterEditorDestroyed() != 0) return 1;
    if (testEditorStateRoundtrip() != 0) return 1;
    std::cout << "All layout tests passed.\n";
    return 0;
}

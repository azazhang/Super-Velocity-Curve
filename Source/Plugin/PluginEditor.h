#pragma once

#include "../Standalone/StandaloneMidiPanel.h"
#include "../UI/AboutPanelComponent.h"
#include "../UI/UnsavedProfileDialogComponent.h"
#include "../UI/CalibrationWizardComponent.h"
#include "../UI/CollapsibleSection.h"
#include "../UI/CurveEditorComponent.h"
#include "../UI/HistogramComponent.h"
#include "../UI/HistogramPairPanel.h"
#include "../UI/LookAndFeel.h"
#include "../UI/MidiActivityMeterComponent.h"
#include "../UI/MidiRoutingPanel.h"
#include "../UI/NoteRemapEditorComponent.h"
#include "../UI/PadGridComponent.h"
#include "../UI/PadInspectorComponent.h"
#include "../UI/Theme.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>
#include <optional>

class SuperVelocityCurveAudioProcessorEditor : public juce::AudioProcessorEditor,
                                               private juce::Timer
{
public:
    explicit SuperVelocityCurveAudioProcessorEditor (SuperVelocityCurveAudioProcessor&);
    ~SuperVelocityCurveAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Called after host `setStateInformation` when this editor is still open. */
    void syncFromProcessorState();

private:
    SuperVelocityCurveAudioProcessor& audioProcessor;

    juce::Label titleLabel { {}, "Super VelocityCurve" };
    juce::Label subtitleLabel { {}, "Per-pad velocity curves for finger drummers" };
    juce::ComboBox profileBox;
    juce::ComboBox outputModeBox;
    juce::ComboBox curvePresetBox;
    juce::TextEditor profileNameEditor;
    juce::TextButton saveProfileButton { "Save Profile" };
    juce::TextButton duplicateProfileButton { "Duplicate" };
    juce::TextButton deleteProfileButton { "Delete" };
    juce::TextButton importButton { "Import" };
    juce::TextButton exportButton { "Export" };
    juce::TextButton resetCurveButton { "Reset" };
    juce::TextButton copyCurveButton { "Copy" };
    juce::TextButton pasteCurveButton { "Paste" };
    juce::TextButton pasteGroupButton { "Paste Group" };
    juce::TextButton captureAbButton { "Capture A" };
    juce::TextButton abToggleButton { "Hear A" };
    juce::TextButton clearHistogramButton { "Clear Hist" };
    juce::TextButton aboutButton { "About" };
    std::unique_ptr<AboutPanelComponent> aboutPanel;
    std::unique_ptr<UnsavedProfileDialogComponent> unsavedProfileDialog;
    juce::Label profileLabel { {}, "Profile" };
    juce::Label outputModeLabel { {}, "MIDI output" };
    juce::Label presetLabel { {}, "Curve preset" };
    juce::Label liveHitsLabel { {}, "Live" };
    juce::Label themeLabel { {}, "Appearance" };
    juce::ComboBox themeBox;

    juce::TabbedComponent midiToolsTabs { juce::TabbedButtonBar::TabsAtTop };

    PadGridComponent padGrid;
    CurveEditorComponent curveEditor;
    PadInspectorComponent padInspector;
    HistogramComponent padHistogram;
    HistogramComponent globalHistogram;
    HistogramPairPanel histogramPair { padHistogram, globalHistogram };
    CalibrationWizardComponent calibrationWizard;

    CollapsibleSection histogramSection { "Histograms", histogramPair, 108, false };
    CollapsibleSection midiToolsSection { "MIDI routing & remap", midiToolsTabs, 168, false };
    CollapsibleSection calibrationSection { "Calibration wizard", calibrationWizard, 120, false };
    CollapsibleSection padSettingsSection { "Pad settings", padInspector, 220, true };
    MidiRoutingPanel midiRoutingPanel;
    NoteRemapEditorComponent noteRemapEditor;
    MidiActivityMeterComponent midiMeters;
    std::unique_ptr<StandaloneMidiPanel> standaloneMidiPanel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> outputModeAttachment;

    int selectedPadIndex = 0;
    bool hearingCurveA = false;
    std::optional<svc::VelocityCurve> clipboardCurve;
    std::optional<svc::VelocityCurve> curveA;

    void timerCallback() override;
    void rebuildProfileList();
    void onProfileSelected();
    void attemptProfileSwitch (int profileBoxId);
    void performProfileSwitch (int profileBoxId);
    void revertActiveProfileFromStore();
    void captureProfileBaseline();
    bool isProfileDirty() const;
    bool saveProfileFromUI (juce::String* errorMessage = nullptr);
    void onPadSelected (int padIndex);
    void refreshPadUI (bool resetPadSelection = false);
    void showPadAtIndex (int padIndex);
    svc::ProfilePad mergeActivePadFromUI() const;
    void commitActivePadEdits();
    void updateSelectedPadFromUI (int padIndex, const svc::ProfilePad& pad, bool syncEngine = true);
    void applyProfileToEngine();
    void syncAbAuditionIfActive();
    void updateLiveHits();
    void updateHistograms();
    void showStatus (const juce::String& message, bool isError = false);
    void toggleAbCurve();
    void applyListenCurveToEngine (const svc::VelocityCurve& curve);
    void clearAbCompare();
    void refreshRoutingPanels();
    void applyThemeFromUI();
    void refreshThemedComponents();
    void showAboutPanel();
    void hideAboutPanel();
    void showUnsavedProfileDialog();
    void hideUnsavedProfileDialog();
    void handleUnsavedProfileChoice (UnsavedProfileDialogComponent::Choice choice);
    bool isPadMapped (int note, int channel) const;
    void layoutBottomSections (juce::Rectangle<int> area);
    int bottomSectionsHeight() const noexcept;
    void syncCurveEditTargetUI();

    juce::String statusMessage;
    bool statusIsError = false;

    svc::ControllerProfile profileBaseline;
    juce::String profileBaselineName;
    bool suppressProfileBoxChange = false;
    int suppressProfileStoreNotifications = 0;
    int pendingProfileBoxId = 0;
    int histogramRefreshCooldown = 0;
    juce::String persistedLiveHitsText;
    int liveHitsDisplayTicksRemaining = 0;

    svc::ui::AppLookAndFeel appLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperVelocityCurveAudioProcessorEditor)
};

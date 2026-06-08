#include "PluginEditor.h"
#include <juce_gui_basics/juce_gui_basics.h>

SuperVelocityCurveAudioProcessorEditor::SuperVelocityCurveAudioProcessorEditor (SuperVelocityCurveAudioProcessor& p)
    : AudioProcessorEditor (p),
      audioProcessor (p)
{
    setLookAndFeel (&appLookAndFeel);
    setResizable (true, true);
    setResizeLimits (1100, 760, 1900, 1150);
    setSize (1280, 860);

    titleLabel.setFont (svc::ui::Theme::titleFont());
    subtitleLabel.setFont (svc::ui::Theme::smallFont());
    subtitleLabel.setColour (juce::Label::textColourId, juce::Colour (svc::ui::Theme::textSecondary));

    for (auto* c : { &titleLabel, &subtitleLabel, &profileLabel, &outputModeLabel, &presetLabel, &liveHitsLabel })
        addAndMakeVisible (c);

    for (auto* c : { &profileBox, &outputModeBox, &curvePresetBox })
        addAndMakeVisible (c);

    profileNameEditor.setText ("My Profile");
    profileNameEditor.setFont (svc::ui::Theme::bodyFont());
    addAndMakeVisible (profileNameEditor);

    for (auto* b : { &saveProfileButton, &duplicateProfileButton, &deleteProfileButton,
                     &importButton, &exportButton, &resetCurveButton, &copyCurveButton,
                     &pasteCurveButton, &pasteGroupButton, &captureAbButton, &abToggleButton,
                     &clearHistogramButton })
        addAndMakeVisible (b);

    outputModeBox.addItemList ({ "Auto (match input)", "MIDI 1.0 (7-bit)", "MIDI 2.0 (high-res)" }, 1);
    curvePresetBox.addItemList ({ "Linear", "Soft", "Hard", "S-Curve", "Exponential", "Logarithmic", "Power" }, 1);
    curvePresetBox.setSelectedId (1, juce::dontSendNotification);

    padHistogram.setTitle ("Pad Histogram");
    globalHistogram.setTitle ("Global Histogram");
    addAndMakeVisible (padGrid);
    addAndMakeVisible (curveEditor);
    addAndMakeVisible (padInspector);
    addAndMakeVisible (padHistogram);
    addAndMakeVisible (globalHistogram);
    addAndMakeVisible (calibrationWizard);
    addAndMakeVisible (midiRoutingPanel);
    addAndMakeVisible (noteRemapEditor);
    addAndMakeVisible (midiMeters);

    if (audioProcessor.wrapperType == juce::AudioProcessor::wrapperType_Standalone)
    {
        standaloneMidiPanel = std::make_unique<StandaloneMidiPanel>();
        addAndMakeVisible (*standaloneMidiPanel);
        standaloneMidiPanel->onMidiMessage = [this] (const juce::MidiMessage& msg)
        {
            audioProcessor.injectStandaloneMidi (msg);
        };
        standaloneMidiPanel->onOutputDeviceChanged = [this] (juce::MidiOutput* out)
        {
            audioProcessor.setStandaloneMidiOutput (out);
        };
    }

    outputModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.getApvts(), "outputMode", outputModeBox);

    padGrid.onPadSelected = [this] (int index) { onPadSelected (index); };
    curveEditor.onPadChanged = [this] (const svc::ProfilePad& pad) { updateSelectedPadFromUI (pad); };

    padInspector.onPadChanged = [this] (int index, const svc::ProfilePad& pad)
    {
        juce::ignoreUnused (index);
        updateSelectedPadFromUI (pad);
        curveEditor.setPad (pad);
    };

    padInspector.onEditAftertouchRequested = [this]
    {
        curveEditor.setEditTarget (CurveEditorComponent::EditTarget::aftertouch);
        showStatus ("Editing aftertouch curve.");
    };

    profileBox.onChange = [this] { onProfileSelected(); };

    curvePresetBox.onChange = [this]
    {
        curveEditor.applyPreset (static_cast<svc::CurvePreset> (curvePresetBox.getSelectedItemIndex()));
    };

    resetCurveButton.onClick = [this] { curveEditor.resetCurve(); };

    copyCurveButton.onClick = [this]
    {
        clipboardCurve = curveEditor.getPad().curve;
        showStatus ("Curve copied.");
    };

    pasteCurveButton.onClick = [this]
    {
        if (! clipboardCurve.has_value())
        {
            showStatus ("Nothing to paste.", true);
            return;
        }

        auto pad = curveEditor.getPad();
        pad.curve = *clipboardCurve;
        updateSelectedPadFromUI (pad);
        curveEditor.setPad (pad);
        showStatus ("Curve pasted.");
    };

    pasteGroupButton.onClick = [this]
    {
        if (! clipboardCurve.has_value())
        {
            showStatus ("Copy a curve first.", true);
            return;
        }

        auto& profile = audioProcessor.getProfileStore().getActiveProfile();
        if (selectedPadIndex < 0 || selectedPadIndex >= static_cast<int> (profile.getPads().size()))
            return;

        const auto group = profile.getPads()[static_cast<size_t> (selectedPadIndex)].group;
        int count = 0;
        for (auto& pad : profile.getPads())
        {
            if (pad.group == group)
            {
                pad.curve = *clipboardCurve;
                ++count;
            }
        }

        audioProcessor.getProfileStore().syncActiveUserProfileFromEdits();
        applyProfileToEngine();
        refreshPadUI();
        showStatus ("Pasted curve to " + juce::String (count) + " pads in group " + svc::padGroupToString (group));
    };

    captureAbButton.onClick = [this]
    {
        curveA = curveEditor.getPad().curve;
        curveB.reset();
        hearingCurveA = false;
        abToggleButton.setButtonText ("Hear A");
        showStatus ("Curve A captured. Edit, then Hear A/B to compare.");
    };

    abToggleButton.onClick = [this] { toggleAbCurve(); };

    clearHistogramButton.onClick = [this]
    {
        audioProcessor.getEngine().clearHistogram();
        updateHistograms();
    };

    calibrationWizard.onCurveCalibrated = [this] (const svc::VelocityCurve& curve)
    {
        auto pad = curveEditor.getPad();
        pad.curve = curve;
        updateSelectedPadFromUI (pad);
        curveEditor.setPad (pad);
        showStatus ("Calibration applied.");
    };

    midiRoutingPanel.onRoutingChanged = [this]
    {
        audioProcessor.getProfileStore().syncActiveUserProfileFromEdits();
        applyProfileToEngine();
        refreshRoutingPanels();
    };

    noteRemapEditor.onRemapsChanged = [this]
    {
        audioProcessor.getProfileStore().syncActiveUserProfileFromEdits();
        applyProfileToEngine();
    };

    saveProfileButton.onClick = [this]
    {
        const auto name = profileNameEditor.getText().trim();
        if (audioProcessor.getProfileStore().saveActiveAsUserProfile (name))
        {
            rebuildProfileList();
            applyProfileToEngine();
            showStatus ("Saved profile: " + name);
        }
        else
            showStatus ("Enter a profile name to save.", true);
    };

    duplicateProfileButton.onClick = [this]
    {
        if (audioProcessor.getProfileStore().duplicateActiveAsUserProfile ({ }))
        {
            rebuildProfileList();
            showStatus ("Profile duplicated.");
        }
    };

    deleteProfileButton.onClick = [this]
    {
        auto& store = audioProcessor.getProfileStore();
        if (store.getActiveEntryType() != svc::ProfileEntryType::userProfile)
        {
            showStatus ("Only user profiles can be deleted.", true);
            return;
        }

        if (store.deleteUserProfile (store.getActiveEntryIndex()))
        {
            rebuildProfileList();
            onPadSelected (0);
            applyProfileToEngine();
            showStatus ("Profile deleted.");
        }
    };

    importButton.onClick = [this]
    {
        auto chooser = std::make_shared<juce::FileChooser> ("Import profile", juce::File{}, "*.xml;*.svcp");
        chooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                              [this, chooser] (const juce::FileChooser& fc)
                              {
                                  const auto file = fc.getResult();
                                  if (file.existsAsFile()
                                      && audioProcessor.getProfileStore().importProfileFromFile (file))
                                  {
                                      rebuildProfileList();
                                      refreshRoutingPanels();
                                      onPadSelected (0);
                                      applyProfileToEngine();
                                      showStatus ("Imported " + file.getFileName());
                                  }
                                  else if (file != juce::File())
                                      showStatus ("Import failed.", true);
                              });
    };

    exportButton.onClick = [this]
    {
        auto chooser = std::make_shared<juce::FileChooser> ("Export profile", juce::File{}, "*.svcp");
        chooser->launchAsync (juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
                              [this, chooser] (const juce::FileChooser& fc)
                              {
                                  auto file = fc.getResult();
                                  if (file == juce::File())
                                      return;

                                  if (! file.hasFileExtension (".svcp") && ! file.hasFileExtension (".xml"))
                                      file = file.withFileExtension (".svcp");

                                  if (audioProcessor.getProfileStore().exportActiveProfileToFile (file))
                                      showStatus ("Exported to " + file.getFileName());
                                  else
                                      showStatus ("Export failed.", true);
                              });
    };

    audioProcessor.getProfileStore().onProfileChanged = [this]
    {
        rebuildProfileList();
        refreshPadUI();
        refreshRoutingPanels();
    };

    rebuildProfileList();
    refreshRoutingPanels();
    onPadSelected (0);
    startTimerHz (30);
}

void SuperVelocityCurveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (svc::ui::Theme::background));

    auto header = getLocalBounds().removeFromTop (56).reduced (16, 10);
    g.setColour (juce::Colour (svc::ui::Theme::textPrimary));
    g.setFont (svc::ui::Theme::titleFont());
    g.drawText (titleLabel.getText(), header.removeFromTop (26), juce::Justification::centredLeft);
    g.setFont (svc::ui::Theme::smallFont());
    g.setColour (juce::Colour (svc::ui::Theme::textSecondary));
    g.drawText (subtitleLabel.getText(), header, juce::Justification::centredLeft);

    if (statusMessage.isNotEmpty())
    {
        g.setColour (statusIsError ? juce::Colours::salmon : juce::Colour (svc::ui::Theme::success));
        g.drawText (statusMessage, getLocalBounds().removeFromBottom (22).reduced (16, 0), juce::Justification::centredLeft);
    }
}

void SuperVelocityCurveAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (12).withTrimmedTop (56).withTrimmedBottom (statusMessage.isEmpty() ? 0 : 22);

    if (standaloneMidiPanel != nullptr)
    {
        standaloneMidiPanel->setBounds (bounds.removeFromTop (88));
        bounds.removeFromTop (4);
    }

    auto toolbar = bounds.removeFromTop (140);
    auto col = toolbar.removeFromLeft (toolbar.getWidth() / 2).reduced (4);
    profileLabel.setBounds (col.removeFromTop (16));
    profileBox.setBounds (col.removeFromTop (24));
    col.removeFromTop (4);
    profileNameEditor.setBounds (col.removeFromTop (24));
    col.removeFromTop (4);
    auto profileButtons = col.removeFromTop (24);
    const int btnW = profileButtons.getWidth() / 5;
    saveProfileButton.setBounds (profileButtons.removeFromLeft (btnW).reduced (1));
    duplicateProfileButton.setBounds (profileButtons.removeFromLeft (btnW).reduced (1));
    deleteProfileButton.setBounds (profileButtons.removeFromLeft (btnW).reduced (1));
    importButton.setBounds (profileButtons.removeFromLeft (btnW).reduced (1));
    exportButton.setBounds (profileButtons.reduced (1));

    col = toolbar.reduced (4);
    auto topRow = col.removeFromTop (44);
    auto outCol = topRow.removeFromLeft (topRow.getWidth() / 2);
    outputModeLabel.setBounds (outCol.removeFromTop (16));
    outputModeBox.setBounds (outCol);
    midiMeters.setBounds (topRow.reduced (2));

    auto presetCol = col.removeFromTop (48);
    presetLabel.setBounds (presetCol.removeFromTop (16));
    auto presetRow = presetCol.removeFromTop (24);
    curvePresetBox.setBounds (presetRow.removeFromLeft (presetRow.getWidth() - 340).reduced (0, 0));
    const int smallBtn = 44;
    resetCurveButton.setBounds (presetRow.removeFromLeft (smallBtn).reduced (1));
    copyCurveButton.setBounds (presetRow.removeFromLeft (smallBtn).reduced (1));
    pasteCurveButton.setBounds (presetRow.removeFromLeft (smallBtn).reduced (1));
    pasteGroupButton.setBounds (presetRow.removeFromLeft (56).reduced (1));
    captureAbButton.setBounds (presetRow.removeFromLeft (56).reduced (1));
    abToggleButton.setBounds (presetRow.reduced (1));

    liveHitsLabel.setBounds (col.removeFromTop (32));
    auto routingCol = col.removeFromTop (col.getHeight() / 2);
    midiRoutingPanel.setBounds (routingCol);
    noteRemapEditor.setBounds (col);

    auto bottom = bounds.removeFromBottom (150);
    auto histRow = bottom.removeFromTop (bottom.getHeight() - 4);
    padHistogram.setBounds (histRow.removeFromLeft (histRow.getWidth() / 2).reduced (2));
    globalHistogram.setBounds (histRow.reduced (2));
    clearHistogramButton.setBounds (bottom.removeFromBottom (24).reduced (2));
    calibrationWizard.setBounds (bottom.reduced (2));

    auto left = bounds.removeFromLeft (juce::jmax (280, bounds.getWidth() * 2 / 5)).reduced (4);
    padGrid.setBounds (left);

    auto right = bounds.reduced (4);
    auto inspector = right.removeFromBottom (juce::jmax (200, right.getHeight() / 3));
    padInspector.setBounds (inspector);
    curveEditor.setBounds (right);
}

void SuperVelocityCurveAudioProcessorEditor::rebuildProfileList()
{
    profileBox.clear();
    const auto entries = audioProcessor.getProfileStore().getProfileList();

    int selectedId = 1;
    int id = 1;
    for (const auto& entry : entries)
    {
        profileBox.addItem (entry.displayName, id);
        if (entry.type == audioProcessor.getProfileStore().getActiveEntryType()
            && entry.index == audioProcessor.getProfileStore().getActiveEntryIndex())
            selectedId = id;
        ++id;
    }

    profileBox.setSelectedId (selectedId, juce::dontSendNotification);
}

void SuperVelocityCurveAudioProcessorEditor::refreshRoutingPanels()
{
    auto& profile = audioProcessor.getProfileStore().getActiveProfile();
    midiRoutingPanel.setProfile (profile);
    noteRemapEditor.setProfile (profile);
}

bool SuperVelocityCurveAudioProcessorEditor::isPadMapped (int note, int channel) const
{
    for (const auto& pad : audioProcessor.getProfileStore().getActiveProfile().getPads())
    {
        if (pad.midiNote == note && pad.midiChannel == channel)
            return true;
    }
    return false;
}

void SuperVelocityCurveAudioProcessorEditor::onProfileSelected()
{
    const auto id = profileBox.getSelectedId();
    if (id <= 0)
        return;

    const auto entries = audioProcessor.getProfileStore().getProfileList();
    const auto index = id - 1;
    if (index < 0 || index >= static_cast<int> (entries.size()))
        return;

    const auto& entry = entries[static_cast<size_t> (index)];
    if (entry.type == svc::ProfileEntryType::factoryTemplate)
        audioProcessor.getProfileStore().loadFactoryTemplate (entry.index);
    else
        audioProcessor.getProfileStore().loadUserProfile (entry.index);

    curveA.reset();
    curveB.reset();
    hearingCurveA = false;
    abToggleButton.setButtonText ("Hear A");
    curveEditor.setEditTarget (CurveEditorComponent::EditTarget::velocity);

    onPadSelected (0);
    applyProfileToEngine();
    refreshRoutingPanels();
}

void SuperVelocityCurveAudioProcessorEditor::onPadSelected (int padIndex)
{
    selectedPadIndex = padIndex;
    padGrid.setSelectedPadIndex (padIndex);
    calibrationWizard.reset();
    curveEditor.setEditTarget (CurveEditorComponent::EditTarget::velocity);
    refreshPadUI();
}

void SuperVelocityCurveAudioProcessorEditor::refreshPadUI()
{
    const auto& profile = audioProcessor.getProfileStore().getActiveProfile();
    padGrid.setProfile (profile);

    if (selectedPadIndex >= 0 && selectedPadIndex < static_cast<int> (profile.getPads().size()))
    {
        const auto& pad = profile.getPads()[static_cast<size_t> (selectedPadIndex)];
        curveEditor.setPad (pad);
        padInspector.setPad (pad, selectedPadIndex);
        padHistogram.setTitle ("Pad: " + pad.label);
    }
}

void SuperVelocityCurveAudioProcessorEditor::updateSelectedPadFromUI (const svc::ProfilePad& pad)
{
    auto& profile = audioProcessor.getProfileStore().getActiveProfile();
    if (selectedPadIndex < 0 || selectedPadIndex >= static_cast<int> (profile.getPads().size()))
        return;

    profile.getPads()[static_cast<size_t> (selectedPadIndex)] = pad;
    audioProcessor.getProfileStore().syncActiveUserProfileFromEdits();
    applyProfileToEngine();
    padInspector.setPad (pad, selectedPadIndex);
}

void SuperVelocityCurveAudioProcessorEditor::applyProfileToEngine()
{
    audioProcessor.applyProfileToEngine();
}

void SuperVelocityCurveAudioProcessorEditor::toggleAbCurve()
{
    if (! curveA.has_value())
    {
        showStatus ("Capture A first.", true);
        return;
    }

    auto pad = curveEditor.getPad();

    if (! curveB.has_value())
        curveB = pad.curve;

    hearingCurveA = ! hearingCurveA;
    pad.curve = hearingCurveA ? *curveA : *curveB;
    abToggleButton.setButtonText (hearingCurveA ? "Hearing A" : "Hearing B");

    updateSelectedPadFromUI (pad);
    curveEditor.setPad (pad);
}

void SuperVelocityCurveAudioProcessorEditor::showStatus (const juce::String& message, bool isError)
{
    statusMessage = message;
    statusIsError = isError;
    resized();
    repaint();
}

void SuperVelocityCurveAudioProcessorEditor::updateHistograms()
{
    const auto& profile = audioProcessor.getProfileStore().getActiveProfile();
    if (selectedPadIndex >= 0 && selectedPadIndex < static_cast<int> (profile.getPads().size()))
    {
        const auto& pad = profile.getPads()[static_cast<size_t> (selectedPadIndex)];
        padHistogram.setHistogram (audioProcessor.getEngine().getPadHistogramSnapshot (pad.midiNote, pad.midiChannel));
    }

    globalHistogram.setHistogram (audioProcessor.getEngine().getGlobalHistogramSnapshot());
}

void SuperVelocityCurveAudioProcessorEditor::updateLiveHits()
{
    svc::HitEvent hit;
    juce::String text;
    bool sawUnmapped = false;

    int count = 0;
    while (audioProcessor.getEngine().getHitFifo().pop (hit) && count < 8)
    {
        const auto inVel = juce::String (static_cast<int> (std::lround (hit.inputVelocity * 127.0f)));
        const auto outVel = juce::String (static_cast<int> (std::lround (hit.outputVelocity * 127.0f)));
        const auto protocol = hit.isMidi2 ? "M2" : "M1";
        text += "N" + juce::String (hit.note) + " " + protocol + " " + inVel + "->" + outVel + "   ";

        curveEditor.addHitMarker (hit.note, hit.channel, hit.inputVelocity, hit.outputVelocity, hit.isMidi2);
        padGrid.flashPadHit (hit.note, hit.channel, hit.outputVelocity);
        calibrationWizard.captureHit (hit.inputVelocity);
        midiMeters.pushInputLevel (hit.inputVelocity);
        midiMeters.pushOutputLevel (hit.outputVelocity);

        if (! isPadMapped (hit.note, hit.channel))
            sawUnmapped = true;

        ++count;
    }

    if (text.isEmpty())
        text = "Play your controller - live input->output velocity appears here";
    else if (sawUnmapped)
        text += "  [unmapped note]";

    liveHitsLabel.setText (text, juce::dontSendNotification);
    updateHistograms();
}

void SuperVelocityCurveAudioProcessorEditor::timerCallback()
{
    updateLiveHits();
    padGrid.decayHitVisuals();
    midiMeters.decay();
    curveEditor.repaint();
}

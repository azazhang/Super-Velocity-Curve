#pragma once

#include "../Profiles/ControllerProfile.h"
#include "Theme.h"
#include <JuceHeader.h>
#include <functional>

class NoteRemapEditorComponent : public juce::Component,
                                 private juce::TableListBoxModel
{
public:
    NoteRemapEditorComponent();

    void setProfile (svc::ControllerProfile& profile);
    void applyCellEdit (int row, int column, const juce::String& text);
    std::function<void()> onRemapsChanged;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    svc::ControllerProfile* profile = nullptr;
    std::vector<svc::NoteRemapEntry> entries;

    juce::TableListBox table;
    juce::TextButton addButton { "Add remap" };
    juce::TextButton clearButton { "Clear all" };
    juce::Label helpLabel;

    void rebuildEntriesFromProfile();
    void syncProfileFromEntries();
    int getNumRows() override;
    void paintRowBackground (juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existing) override;
    void cellClicked (int rowNumber, int columnId, const juce::MouseEvent&) override;
};

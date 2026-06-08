#include "NoteRemapEditorComponent.h"

namespace
{
class NoteCellEditor : public juce::TextEditor
{
public:
    NoteCellEditor (NoteRemapEditorComponent& owner, int row, int col)
        : ownerRef (owner), rowIndex (row), columnIndex (col)
    {
        setInputRestrictions (3, "0123456789");
        onReturnKey = onFocusLost = [this] { ownerRef.applyCellEdit (rowIndex, columnIndex, getText()); };
    }

    NoteRemapEditorComponent& ownerRef;
    int rowIndex;
    int columnIndex;
};

class ChannelCellEditor : public juce::ComboBox
{
public:
    ChannelCellEditor (NoteRemapEditorComponent& owner, int row, int col)
        : ownerRef (owner), rowIndex (row), columnIndex (col)
    {
        for (int ch = 0; ch <= 16; ++ch)
            addItem (ch == 0 ? "Any" : "Ch " + juce::String (ch), ch + 1);
        onChange = [this]
        {
            ownerRef.applyCellEdit (rowIndex, columnIndex, juce::String (getSelectedId() - 1));
        };
    }

    NoteRemapEditorComponent& ownerRef;
    int rowIndex;
    int columnIndex;
};
} // namespace

NoteRemapEditorComponent::NoteRemapEditorComponent()
{
    table.setModel (this);
    addAndMakeVisible (table);
    addAndMakeVisible (addButton);
    addAndMakeVisible (clearButton);
    addAndMakeVisible (helpLabel);

    helpLabel.setFont (svc::ui::Theme::smallFont());
    helpLabel.setText ("Remaps apply before velocity curves. Channel 0 = any.", juce::dontSendNotification);

    table.getHeader().addColumn ("Src Note", 1, 60);
    table.getHeader().addColumn ("Src Ch", 2, 55);
    table.getHeader().addColumn ("Tgt Note", 3, 60);
    table.getHeader().addColumn ("Tgt Ch", 4, 55);

    addButton.onClick = [this]
    {
        entries.push_back ({ 60, 10, 36, 10 });
        syncProfileFromEntries();
        table.updateContent();
    };

    clearButton.onClick = [this]
    {
        entries.clear();
        syncProfileFromEntries();
        table.updateContent();
    };
}

void NoteRemapEditorComponent::setProfile (svc::ControllerProfile& p)
{
    profile = &p;
    rebuildEntriesFromProfile();
    table.updateContent();
}

void NoteRemapEditorComponent::rebuildEntriesFromProfile()
{
    entries.clear();
    if (profile == nullptr)
        return;

    const auto& routing = profile->getMidiRouting();
    for (int i = 0; i < 128; ++i)
    {
        if (routing.noteRemap[static_cast<size_t> (i)].has_value())
            entries.push_back (*routing.noteRemap[static_cast<size_t> (i)]);
    }
}

void NoteRemapEditorComponent::syncProfileFromEntries()
{
    if (profile == nullptr)
        return;

    auto& routing = profile->getMidiRouting();
    routing.clearRemaps();
    for (const auto& entry : entries)
        routing.setRemap (entry.sourceNote, entry.sourceChannel, entry.targetNote, entry.targetChannel);

    if (onRemapsChanged)
        onRemapsChanged();
}

void NoteRemapEditorComponent::applyCellEdit (int row, int column, const juce::String& text)
{
    if (row < 0 || row >= static_cast<int> (entries.size()))
        return;

    auto& entry = entries[static_cast<size_t> (row)];
    const auto value = text.getIntValue();

    switch (column)
    {
        case 1: entry.sourceNote = juce::jlimit (0, 127, value); break;
        case 2: entry.sourceChannel = juce::jlimit (0, 16, value); break;
        case 3: entry.targetNote = juce::jlimit (0, 127, value); break;
        case 4: entry.targetChannel = juce::jlimit (0, 16, value); break;
        default: break;
    }

    syncProfileFromEntries();
}

int NoteRemapEditorComponent::getNumRows()
{
    return static_cast<int> (entries.size());
}

void NoteRemapEditorComponent::paintRowBackground (juce::Graphics& g, int, int width, int height, bool rowIsSelected)
{
    g.fillAll (rowIsSelected ? juce::Colour (svc::ui::Theme::accent()).withAlpha (0.2f)
                             : juce::Colours::transparentBlack);
    g.setColour (juce::Colour (svc::ui::Theme::border()));
    g.drawHorizontalLine (height - 1, 0.0f, static_cast<float> (width));
}

void NoteRemapEditorComponent::paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool)
{
    if (rowNumber < 0 || rowNumber >= static_cast<int> (entries.size()))
        return;

    const auto& entry = entries[static_cast<size_t> (rowNumber)];
    juce::String text;
    switch (columnId)
    {
        case 1: text = juce::String (entry.sourceNote); break;
        case 2: text = entry.sourceChannel == 0 ? "Any" : juce::String (entry.sourceChannel); break;
        case 3: text = juce::String (entry.targetNote); break;
        case 4: text = entry.targetChannel == 0 ? "Keep" : juce::String (entry.targetChannel); break;
        default: break;
    }

    g.setColour (juce::Colour (svc::ui::Theme::textPrimary()));
    g.setFont (svc::ui::Theme::smallFont());
    g.drawText (text, 4, 0, width - 8, height, juce::Justification::centredLeft);
}

juce::Component* NoteRemapEditorComponent::refreshComponentForCell (int rowNumber, int columnId, bool, juce::Component* existing)
{
    if (rowNumber < 0 || rowNumber >= static_cast<int> (entries.size()))
        return nullptr;

    const auto& entry = entries[static_cast<size_t> (rowNumber)];

    if (columnId == 2 || columnId == 4)
    {
        auto* box = dynamic_cast<ChannelCellEditor*> (existing);
        if (box == nullptr)
            box = new ChannelCellEditor (*this, rowNumber, columnId);

        const int ch = columnId == 2 ? entry.sourceChannel : entry.targetChannel;
        box->setSelectedId (ch + 1, juce::dontSendNotification);
        return box;
    }

    auto* editor = dynamic_cast<NoteCellEditor*> (existing);
    if (editor == nullptr)
        editor = new NoteCellEditor (*this, rowNumber, columnId);

    switch (columnId)
    {
        case 1: editor->setText (juce::String (entry.sourceNote), false); break;
        case 3: editor->setText (juce::String (entry.targetNote), false); break;
        default: break;
    }

    return editor;
}

void NoteRemapEditorComponent::cellClicked (int rowNumber, int columnId, const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown() && rowNumber >= 0 && rowNumber < static_cast<int> (entries.size()))
    {
        entries.erase (entries.begin() + rowNumber);
        syncProfileFromEntries();
        table.updateContent();
    }
}

void NoteRemapEditorComponent::paint (juce::Graphics& g)
{
    juce::ignoreUnused (g);
}

void NoteRemapEditorComponent::resized()
{
    auto area = getLocalBounds().reduced (8);
    helpLabel.setBounds (area.removeFromBottom (16));
    auto buttons = area.removeFromBottom (24);
    addButton.setBounds (buttons.removeFromLeft (90).reduced (1));
    clearButton.setBounds (buttons.removeFromLeft (80).reduced (1));
    table.setBounds (area);
}

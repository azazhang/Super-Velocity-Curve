#include "StandaloneMidiPanel.h"
#include "../UI/Theme.h"

StandaloneMidiPanel::StandaloneMidiPanel()
{
    addAndMakeVisible (inputLabel);
    addAndMakeVisible (outputLabel);
    addAndMakeVisible (inputDeviceBox);
    addAndMakeVisible (outputDeviceBox);
    addAndMakeVisible (statusLabel);
    statusLabel.setFont (svc::ui::Theme::smallFont());

    refreshDeviceLists();
    inputDeviceBox.onChange = [this] { connectInput (inputDeviceBox.getSelectedItemIndex()); };
    outputDeviceBox.onChange = [this] { connectOutput (outputDeviceBox.getSelectedItemIndex()); };
}

StandaloneMidiPanel::~StandaloneMidiPanel()
{
    activeInput.reset();
    activeOutput.reset();
}

void StandaloneMidiPanel::refreshDeviceLists()
{
    inputDeviceBox.clear();
    outputDeviceBox.clear();

    const auto inputs = juce::MidiInput::getAvailableDevices();
    int id = 1;
    for (const auto& device : inputs)
        inputDeviceBox.addItem (device.name, id++);

    outputDeviceBox.addItem ("(none)", 1);
    id = 2;
    const auto outputs = juce::MidiOutput::getAvailableDevices();
    for (const auto& device : outputs)
        outputDeviceBox.addItem (device.name, id++);

    if (inputs.isEmpty())
        statusLabel.setText ("No MIDI inputs. Use macOS IAC Driver or Windows loopMIDI.", juce::dontSendNotification);
    else
    {
        inputDeviceBox.setSelectedId (1, juce::dontSendNotification);
        connectInput (0);
    }

    outputDeviceBox.setSelectedId (1, juce::dontSendNotification);
}

void StandaloneMidiPanel::connectInput (int index)
{
    activeInput.reset();
    const auto devices = juce::MidiInput::getAvailableDevices();
    if (index < 0 || index >= devices.size())
        return;

    activeInput = juce::MidiInput::openDevice (devices[index].identifier, this);
    if (activeInput != nullptr)
    {
        activeInput->start();
        statusLabel.setText ("In: " + devices[index].name + " | Use IAC/loopMIDI for virtual routing.", juce::dontSendNotification);
    }
}

void StandaloneMidiPanel::connectOutput (int index)
{
    activeOutput.reset();
    if (index <= 0)
    {
        if (onOutputDeviceChanged)
            onOutputDeviceChanged (nullptr);
        return;
    }

    const auto devices = juce::MidiOutput::getAvailableDevices();
    const auto deviceIndex = index - 1;
    if (deviceIndex < 0 || deviceIndex >= devices.size())
        return;

    activeOutput = juce::MidiOutput::openDevice (devices[deviceIndex].identifier);
    if (onOutputDeviceChanged)
        onOutputDeviceChanged (activeOutput.get());
}

void StandaloneMidiPanel::handleIncomingMidiMessage (juce::MidiInput*, const juce::MidiMessage& message)
{
    if (onMidiMessage)
        onMidiMessage (message);
}

void StandaloneMidiPanel::paint (juce::Graphics& g)
{
    svc::ui::Theme::fillPanel (g, getLocalBounds().toFloat(), 8.0f);
}

void StandaloneMidiPanel::resized()
{
    auto area = getLocalBounds().reduced (8);
    auto row1 = area.removeFromTop (36);
    inputLabel.setBounds (row1.removeFromLeft (80));
    inputDeviceBox.setBounds (row1);
    area.removeFromTop (4);
    auto row2 = area.removeFromTop (36);
    outputLabel.setBounds (row2.removeFromLeft (80));
    outputDeviceBox.setBounds (row2);
    area.removeFromTop (4);
    statusLabel.setBounds (area);
}

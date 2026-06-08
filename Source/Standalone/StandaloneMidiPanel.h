#pragma once

#include <JuceHeader.h>

class StandaloneMidiPanel : public juce::Component,
                            private juce::MidiInputCallback
{
public:
    StandaloneMidiPanel();
    ~StandaloneMidiPanel() override;

    std::function<void (const juce::MidiMessage&)> onMidiMessage;
    std::function<void (juce::MidiOutput*)> onOutputDeviceChanged;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::ComboBox inputDeviceBox;
    juce::ComboBox outputDeviceBox;
    juce::Label inputLabel { {}, "MIDI Input" };
    juce::Label outputLabel { {}, "MIDI Output" };
    juce::Label statusLabel;
    std::unique_ptr<juce::MidiInput> activeInput;
    std::unique_ptr<juce::MidiOutput> activeOutput;

    void refreshDeviceLists();
    void connectInput (int index);
    void connectOutput (int index);
    void handleIncomingMidiMessage (juce::MidiInput*, const juce::MidiMessage& message) override;
};

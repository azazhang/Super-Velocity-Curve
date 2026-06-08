#include "../Source/Engine/MidiUtilities.h"
#include "../Source/Engine/VelocityCurve.h"
#include "../Source/Engine/VelocityEngine.h"
#include "../Source/Profiles/ControllerProfile.h"
#include "../Source/Profiles/PadTypes.h"
#include <cmath>
#include <iostream>

#define EXPECT_TRUE(expr) do { if (! (expr)) { std::cerr << "FAIL: " #expr << " at " << __LINE__ << '\n'; return 1; } } while (0)
#define EXPECT_NEAR(a, b, eps) do { if (std::abs ((a) - (b)) > (eps)) { std::cerr << "FAIL: " #a " vs " #b << " at " << __LINE__ << '\n'; return 1; } } while (0)

static int testVelocityCurveMonotonic()
{
    svc::VelocityCurve curve;
    curve.applyPreset (svc::CurvePreset::soft);
    const auto& lut = curve.getLut();

    for (int i = 1; i < svc::VelocityCurve::lutSize; ++i)
        EXPECT_TRUE (lut[static_cast<size_t> (i)] >= lut[static_cast<size_t> (i - 1)]);

    std::vector<svc::CurveControlPoint> points = {
        { 0.0f, 0.0f }, { 0.3f, 0.8f }, { 0.6f, 0.5f }, { 1.0f, 1.0f }
    };
    curve.setControlPoints (points);
    const auto& lut2 = curve.getLut();
    for (int i = 1; i < svc::VelocityCurve::lutSize; ++i)
        EXPECT_TRUE (lut2[static_cast<size_t> (i)] >= lut2[static_cast<size_t> (i - 1)]);

    return 0;
}

static int testVelocityEnginePerPadRetrigger()
{
    svc::VelocityEngine engine;
    engine.setSampleRate (48000.0);

    svc::PadSettings pad;
    pad.retriggerGuardMs = 50.0;
    pad.enabled = true;
    engine.setPadSettings (36, 10, pad);

    juce::MidiBuffer buffer;
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.8f), 0);
    engine.processMidiBuffer (buffer, 128);

    buffer.clear();
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.9f), 0);
    engine.processMidiBuffer (buffer, 128);

    EXPECT_TRUE (buffer.getNumEvents() == 0);
    return 0;
}

static int testProfileApplyClearsOldPads()
{
    svc::VelocityEngine engine;
    svc::ControllerProfile gm = svc::ControllerProfile::createGMStandard();
    gm.applyToEngine (engine);

    svc::PadSettings gatedPad;
    gatedPad.velocityGate = 0.5f;
    engine.setPadSettings (36, 10, gatedPad);

    juce::MidiBuffer buffer;
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.2f), 0);
    engine.processMidiBuffer (buffer, 64);
    EXPECT_TRUE (buffer.getNumEvents() == 0);

    svc::ControllerProfile small = svc::ControllerProfile::createSpdSx();
    small.applyToEngine (engine);

    buffer.clear();
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.2f), 0);
    engine.processMidiBuffer (buffer, 64);
    EXPECT_TRUE (buffer.getNumEvents() == 1);
    return 0;
}

static int testNoteRemapAndChannelFilter()
{
    svc::MidiRoutingSettings routing;
    routing.inputChannelFilter = 10;
    routing.outputChannel = 10;
    routing.setRemap (60, 0, 36, 10);

    svc::VelocityEngine engine;
    engine.setMidiRouting (routing);

    juce::MidiBuffer buffer;
    buffer.addEvent (juce::MidiMessage::noteOn (9, 60, 0.7f), 0);
    engine.processMidiBuffer (buffer, 64);
    EXPECT_TRUE (buffer.getNumEvents() == 0);

    buffer.clear();
    buffer.addEvent (juce::MidiMessage::noteOn (10, 60, 0.7f), 0);
    engine.processMidiBuffer (buffer, 64);
    EXPECT_TRUE (buffer.getNumEvents() == 1);

    juce::MidiMessage msg;
    for (const auto metadata : buffer)
        msg = metadata.getMessage();

    EXPECT_TRUE (msg.getNoteNumber() == 36);
    EXPECT_TRUE (msg.getChannel() == 10);
    return 0;
}

static int testMidi1OutputMode()
{
    svc::VelocityEngine engine;
    engine.setOutputMode (svc::VelocityOutputMode::midi1);

    svc::PadSettings pad;
    pad.enabled = true;
    engine.setPadSettings (36, 10, pad);

    juce::MidiBuffer buffer;
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.333f), 0);
    engine.processMidiBuffer (buffer, 64);

    juce::MidiMessage msg;
    for (const auto metadata : buffer)
        msg = metadata.getMessage();

    EXPECT_TRUE (msg.getVelocity() == 42);
    return 0;
}

static int testGateClampMode()
{
    svc::VelocityEngine engine;
    svc::PadSettings pad;
    pad.enabled = true;
    pad.velocityGate = 0.2f;
    pad.gateMode = svc::VelocityGateMode::clampToFloor;
  pad.curve.setFloor (0.15f);
    engine.setPadSettings (36, 10, pad);

    juce::MidiBuffer buffer;
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.05f), 0);
    engine.processMidiBuffer (buffer, 64);
    EXPECT_TRUE (buffer.getNumEvents() == 1);
    return 0;
}

static int testPerPadHistogram()
{
    svc::VelocityEngine engine;
    svc::PadSettings pad;
    pad.enabled = true;
    engine.setPadSettings (36, 10, pad);

    juce::MidiBuffer buffer;
    buffer.addEvent (juce::MidiMessage::noteOn (10, 36, 0.5f), 0);
    engine.processMidiBuffer (buffer, 64);

    const auto snap = engine.getPadHistogramSnapshot (36, 10);
    int total = 0;
    for (int i = 0; i < 128; ++i)
        total += snap.inputBins[static_cast<size_t> (i)];
    EXPECT_TRUE (total == 1);
    return 0;
}

static int testLaunchpadProfileSize()
{
    const auto profile = svc::ControllerProfile::createLaunchpadDrumRack();
    EXPECT_TRUE (profile.getPads().size() == 64);
    return 0;
}

int main()
{
    if (testVelocityCurveMonotonic() != 0) return 1;
    if (testVelocityEnginePerPadRetrigger() != 0) return 1;
    if (testProfileApplyClearsOldPads() != 0) return 1;
    if (testNoteRemapAndChannelFilter() != 0) return 1;
    if (testMidi1OutputMode() != 0) return 1;
    if (testGateClampMode() != 0) return 1;
    if (testPerPadHistogram() != 0) return 1;
    if (testLaunchpadProfileSize() != 0) return 1;
    std::cout << "All engine tests passed.\n";
    return 0;
}

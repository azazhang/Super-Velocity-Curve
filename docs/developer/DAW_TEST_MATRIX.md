# DAW test matrix

Manual verification checklist for Super Velocity Curve **v0.5**. Use the plugin variant that matches your DAW.

| DAW | Plugin variant | Insert location | MIDI routing | Status |
|-----|----------------|-----------------|--------------|--------|
| **Ableton Live 11/12** | `Super Velocity Curve.vst3` (Instrument) | **Instrument** slot on MIDI track | Track 2 **MIDI From** → Track 1 → plugin | **Use this** — not MIDI FX VST3 |
| **Ableton Live 11/12** | `Super Velocity Curve.component` (`aumu`) | Instrument slot (AU) | Same MIDI From routing as VST3 | AU instrument type as of v0.2.16 |
| **Ableton Live 11/12** | `Super Velocity Curve MIDI FX.vst3` | — | — | **Unsupported** — Live has no VST3 MIDI-effect chain; plugin has no audio buses |
| **Logic Pro** | `Super Velocity Curve MIDI FX.component` (`aumi`) | MIDI FX slot on software instrument track | Native MIDI FX chain | Preferred |
| **Logic Pro** | `Super Velocity Curve.component` (`aumu`) | Software instrument slot | Native instrument MIDI | Fallback |
| **Reaper** | `Super Velocity Curve MIDI FX.vst3` | Track FX → MIDI | Input monitoring on | Preferred |
| **Bitwig Studio** | `Super Velocity Curve MIDI FX.clap` or `.vst3` | Note FX / MIDI FX | Device chain | CLAP preferred |
| **Studio One** | `Super Velocity Curve MIDI FX.vst3` | Instrument or MIDI FX (host-dependent) | Pre-instrument | Test per version |
| **Standalone** | `Super Velocity Curve.app` / `.exe` | N/A | IAC Driver (macOS) or loopMIDI (Windows) | Virtual MIDI |

## AU type reference

| FourCC | JUCE setting | Use case |
|--------|--------------|----------|
| `aumi` | `IS_MIDI_EFFECT TRUE` | Logic **MIDI FX** slot |
| `aumu` | `AU_MAIN_TYPE kAudioUnitType_MusicDevice` | Ableton / Logic **instrument** slot |

## Validation

All shipped formats are validated in CI with **pluginval strictness level 5**. Logs are uploaded as workflow artifacts (`pluginval-logs-*`).

See [docs/PLUGINVAL.md](PLUGINVAL.md) for the Verified by pluginval program.

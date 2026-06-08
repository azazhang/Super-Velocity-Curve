# DAW test matrix

Manual verification checklist for Super VelocityCurve **v0.5**. Use the plugin variant that matches your DAW.

| DAW | Plugin variant | Insert location | MIDI routing | Status |
|-----|----------------|-----------------|--------------|--------|
| **Ableton Live 11/12** | `Super VelocityCurve.vst3` (Instrument) | Instruments on MIDI track | Track 2 **MIDI From** → Track 1 → plugin | Required workaround |
| **Logic Pro** | `Super VelocityCurve MIDI FX.component` (`aumi`) | MIDI FX slot on software instrument track | Native MIDI FX chain | Preferred |
| **Logic Pro** | `Super VelocityCurve.component` (`aumf`) | Audio / Music Effect insert | Sidechain MIDI if needed | Fallback |
| **Reaper** | `Super VelocityCurve MIDI FX.vst3` | Track FX → MIDI | Input monitoring on | Preferred |
| **Bitwig Studio** | `Super VelocityCurve MIDI FX.clap` or `.vst3` | Note FX / MIDI FX | Device chain | CLAP preferred |
| **Studio One** | `Super VelocityCurve MIDI FX.vst3` | Instrument or MIDI FX (host-dependent) | Pre-instrument | Test per version |
| **Standalone** | `Super VelocityCurve.app` / `.exe` | N/A | IAC Driver (macOS) or loopMIDI (Windows) | Virtual MIDI |

## AU type reference

| FourCC | JUCE setting | Use case |
|--------|--------------|----------|
| `aumi` | `IS_MIDI_EFFECT TRUE` | Logic **MIDI FX** slot |
| `aumf` | `IS_MIDI_EFFECT FALSE` + MIDI I/O | Instrument-track effect workaround |

## Validation

All shipped formats are validated in CI with **pluginval strictness level 5**. Logs are uploaded as workflow artifacts (`pluginval-logs-*`).

See [docs/PLUGINVAL.md](PLUGINVAL.md) for the Verified by pluginval program.

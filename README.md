# Super VelocityCurve

A free, open MIDI utility for finger drummers and pad players. Shape **per-pad velocity curves** with controller-specific profiles that work across any drum instrument plugin.

[![Verified by pluginval](https://www.tracktion.com/develop/pluginval)](https://www.tracktion.com/develop/pluginval)

All shipped plugin formats are validated with [pluginval](https://github.com/Tracktion/pluginval) at **strictness level 5** in CI. See [docs/PLUGINVAL.md](docs/PLUGINVAL.md).

## Features

- Per-pad velocity curves with multi-point editor, presets (incl. power), floor/ceiling (MIDI 0–127)
- Monotonic curve enforcement, calibration wizard with preview, per-pad + global histograms
- Per-pad retrigger guard, velocity gate (drop or clamp-to-floor), aftertouch curve editor
- Note remap table, channel filter/transform, pad groups with group curve paste
- Humanize, library compensation presets, zone routing by pad group
- A/B compare (Capture A / Hear A-B), copy/paste curves, unmapped-note warnings
- User profile system: save, duplicate, import/export (`.svcp` / `.xml`)
- Factory templates: GM, Blank, Launchpad 8×8, Maschine 16, SPD-SX, Yamaha FGDP + Rx
- Standalone MIDI input + output device pickers (use IAC / loopMIDI for virtual ports)
- MIDI 2.0-ready float pipeline with **16384-entry MIDI 2 LUT** (Auto / MIDI 1.0 / MIDI 2.0 output)
- **Dual VST3 builds**, dual AU types, optional **CLAP** Note FX

## Plugin variants

We ship two plugin builds from the same engine — one DAW cannot satisfy both:

| Variant | VST3 | AU | Use when |
|---------|------|-----|----------|
| **Super VelocityCurve** | Instrument (`Svcv`) | `aumf` Music Effect | **Ableton Live**, FL Studio — needs audio bus |
| **Super VelocityCurve MIDI FX** | Fx (`Svc2`) | `aumi` MIDI Processor | **Logic MIDI FX slot**, Reaper, Bitwig |
| **CLAP** (MIDI FX target) | — | — | Bitwig Note FX chain |

### Why `aumf` existed before v0.5 (and why `aumi` is separate)

JUCE maps plugin type to AU fourCC automatically:

- `IS_MIDI_EFFECT FALSE` + MIDI I/O → **`aumf`** (Music Effect) — loads on instrument/audio tracks, **not** Logic’s MIDI FX slot
- `IS_MIDI_EFFECT TRUE` → **`aumi`** (MIDI Processor) — Logic’s native **MIDI FX** slot

v0.4 used only `aumf` because the whole product was forced to **Ableton’s Instrument workaround** (`IS_MIDI_EFFECT FALSE` + stereo bus). That was correct for Ableton but wrong for Logic’s MIDI FX chain. v0.5 adds a dedicated **MIDI FX** target with `aumi` while keeping the Instrument/`aumf` build for Ableton.

## Host setup

See [docs/DAW_TEST_MATRIX.md](docs/DAW_TEST_MATRIX.md) for a full checklist.

### Ableton Live

1. Install `Super VelocityCurve.vst3` (Instrument build only)
2. Preferences → Plug-Ins → rescan VST3
3. Load under **Instruments** on a MIDI track
4. Route processed MIDI to your drum track via **MIDI From**

### Logic Pro

1. Install `Super VelocityCurve MIDI FX.component` (`aumi`)
2. Insert in the **MIDI FX** slot on a software instrument track
3. Alternatively use the `aumf` build on an instrument track if your workflow prefers

### Virtual MIDI (Standalone / routing)

- **macOS:** enable **IAC Driver** in Audio MIDI Setup → create a bus; route Standalone output to your DAW input
- **Windows:** install [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) and select it in Standalone

## Build

```bash
git submodule update --init --recursive
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
cmake --build build --target export-factory-profiles
```

Install locally (macOS): `./scripts/install-macos.sh build/SuperVelocityCurve_artefacts/Release/` (copy both artefact folders into one dist dir first).

## Unsigned distribution

No paid developer accounts required. macOS: `xattr -cr` on bundles or allow in System Settings. CI produces unsigned zip artifacts.

## License

MIT

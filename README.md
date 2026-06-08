# Super VelocityCurve

Free, open-source **MIDI velocity curves** built for finger drummers and pad players — not keyboard players tweaking one global curve.

Shape how hard you hit maps to MIDI velocity **per pad**, with **controller-specific profiles** that travel with you across DAWs and drum plugins.

[![Verified by pluginval](https://assets.tracktion.com/img/pages/develop/develop-logo-pluginval.png)](https://www.tracktion.com/develop/pluginval) — strictness 5 in CI on all formats pluginval can load ([details](docs/developer/PLUGINVAL.md); Windows CLAP is built but not pluginval-scannable).

## Why Super VelocityCurve?

| Problem | Our approach |
|---------|----------------|
| One global velocity curve in your DAW | **Per-pad curves** — soft ghost notes on snare, punchy kicks, different curves per cymbal |
| Pads feel different on Launchpad vs Maschine vs SPD-SX | **Factory profiles** tuned for real layouts (GM, Launchpad 8×8, Maschine, SPD-SX, Yamaha FGDP) |
| Soft hits disappear or loud hits clip | **Input gates** + floor/ceiling — drop below-threshold hits or clamp output range |
| Double-triggering on sensitive pads | **Per-pad retrigger guard** (ms) |
| Aftertouch feels wrong on expressive kits | **Separate aftertouch curves** per pad |
| Moving between DAWs breaks your setup | **Export/import `.svcp` profiles** — your curves are yours |
| MIDI 2.0 controllers arriving | **16384-entry LUT**, Auto / MIDI 1.0 / MIDI 2.0 output modes |

**A/B compare** (Capture A → edit → Hear A/B) lets you audition curve changes before committing. **Live histograms** show where you actually play. **Calibration wizard** builds a curve from your playing. **Note remap** fixes wrong note assignments without re-patching hardware.

## Download & install

**→ [Install guide](docs/user/install.md)** — step-by-step for macOS and Windows (no coding required).

Get the latest zip from **[GitHub Releases](https://github.com/azazhang/Super_VelocityCurve/releases)**.

## Getting started

**→ [Getting started](docs/user/getting-started.md)** — Ableton, Logic, Reaper, Standalone routing.

**Pad setup:** select a pad in **Pad Layout**, then edit its **name, MIDI note, and channel** in **Pad Settings** below the curve editor.

## Features

- Per-pad velocity curves with gate handles, presets, floor/ceiling
- Factory profiles: GM, Launchpad 8×8, Maschine, SPD-SX, Yamaha FGDP
- Calibration wizard, histograms, A/B compare, note remap
- Humanize, library compensation, zone routing by pad group
- **Two plugin builds:** Instrument (Ableton) + MIDI FX (Logic, Reaper, Bitwig)
- Standalone app with MIDI in/out

## Which plugin do I need?

| Your DAW | Download |
|----------|----------|
| **Ableton Live** | `Super VelocityCurve` (Instrument VST3) |
| **Logic Pro** | `Super VelocityCurve MIDI FX` (AU or VST3) |
| **Reaper / Bitwig** | `Super VelocityCurve MIDI FX` |

## Contributing & development

See [docs/developer/CONTRIBUTING.md](docs/developer/CONTRIBUTING.md) and [docs/developer/TESTING.md](docs/developer/TESTING.md). AI agent notes: [AGENTS.md](AGENTS.md).

## License

MIT

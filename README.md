# Super Velocity Curve

**Per-pad MIDI velocity curves** — when your DAW’s single global curve is not enough.

Free and open source. Built around **pad grids and drum layouts**, but useful anywhere you need **different dynamics per note**: finger drumming, electronic kits, hybrid percussion, inconsistent controllers, or teaching soft vs loud hits.

> **Status: beta (v0.2.x)** — core workflow works; expect rough edges. [Known limitations](#known-limitations) below.

## Support the project

If this helps your playing or teaching:

- [Buy Me a Coffee](https://buymeacoffee.com/azhang)
- [Ko-fi — Studio J](https://ko-fi.com/studioj)

<a href="https://www.tracktion.com/develop/pluginval"><img src="https://assets.tracktion.com/img/pages/develop/develop-logo-pluginval.png" alt="Verified by pluginval" width="120" /></a> — automated validation at strictness 5 (VST3/AU in CI).

## Who is this for?

| You | What you get |
|-----|----------------|
| **Finger drummers** (Launchpad, Maschine, FGDP, SamplePad) | Factory layouts, per-pad curves, ghost-note gates, retrigger guard |
| **E-kit players** routing into VST drums | Zone curves — soft rims, punchy kicks, different cymbal weights |
| **Producers** with uneven pad sensitivity | Shape dynamics *before* the sampler; histograms show what you actually play |
| **Teachers & demo** | Repeatable soft/loud tiers; profiles you can share as `.svcp` files |
| **Multi-DAW users** | Same profile in Logic, Reaper, Bitwig, Ableton — export/import, not re-tweaking |

**Velocity curve** is the term musicians already know. Super Velocity Curve means **better than one curve for the whole keyboard**.

## Why not just use my DAW curve?

| Problem | Our approach |
|---------|----------------|
| One global velocity curve in your DAW | **Per-pad curves** — ghost snares, punchy kicks, separate cymbal weight |
| Pads feel different on Launchpad vs Maschine vs SPD-SX | **Factory profiles** for real layouts (GM, Launchpad 8×8, Maschine, SPD-SX, Yamaha FGDP) |
| Soft hits disappear or loud hits clip | **Input gates** + floor/ceiling — drop or clamp out-of-range hits |
| Double-triggering on sensitive pads | **Per-pad retrigger guard** (ms) |
| Aftertouch feels wrong on expressive kits | **Separate aftertouch curves** per pad |
| Moving between DAWs breaks your setup | **Export/import `.svcp` profiles** |
| MIDI 2.0 controllers arriving | **16384-entry LUT**, Auto / MIDI 1.0 / MIDI 2.0 output modes |

**A/B compare**, **live histograms**, **calibration wizard**, and **note remap** round out the workflow.

## Download & install

**→ [Install guide](docs/user/install.md)** — copy plugins, rescan, unsigned-build tips.

**Latest release:** [github.com/azazhang/SuperVelocityCurve/releases/latest](https://github.com/azazhang/SuperVelocityCurve/releases/latest) — macOS and Windows zips (VST3 + Standalone; AU on Mac; CLAP MIDI FX on **both** platforms).

## Getting started

**→ [Getting started](docs/user/getting-started.md)** — per-DAW routing, curve editor, profiles, save/export.

## Which plugin do I need?

Two builds per release: **Instrument** (Ableton) and **MIDI FX** (most other DAWs).

| Format | Instrument | MIDI FX | Platforms |
|--------|------------|---------|-----------|
| VST3 | ✓ | ✓ | macOS, Windows |
| AU | ✓ | ✓ | macOS only |
| CLAP | — | ✓ | macOS, Windows |
| Standalone | ✓ | — | macOS app, Windows exe |

**Why no CLAP Instrument?** By design. CLAP hosts load MIDI FX as `note-effect` in the chain (Reaper, Bitwig, Logic). Ableton needs an **Instrument** slot and does not load CLAP — that build stays VST3/AU.

| DAW | Plugin name | Formats | Where to load |
|-----|-------------|---------|---------------|
| Ableton Live | Super Velocity Curve | VST3 Instrument | MIDI track (instrument slot — not MIDI FX) |
| Logic Pro | Super Velocity Curve MIDI FX | AU | MIDI FX slot above the instrument |
| Reaper | Super Velocity Curve MIDI FX | VST3, CLAP; AU on macOS | Track input FX (before instrument) |
| Bitwig | Super Velocity Curve MIDI FX | CLAP (preferred), VST3 | Note FX chain |
| FL Studio | Super Velocity Curve MIDI FX | VST3 | MIDI effect on channel (before sampler) |
| Cubase / Nuendo | Super Velocity Curve MIDI FX | VST3 | MIDI Modifiers / MIDI plugin slot |
| Studio One | Super Velocity Curve MIDI FX | VST3 | Event FX / MIDI FX (version-dependent) |
| Standalone | Super Velocity Curve | app / exe | IAC (Mac) or loopMIDI (Windows) |

## Features

- Per-pad velocity curves with gate handles, presets, floor/ceiling
- Factory profiles: GM, Launchpad 8×8, Maschine, SPD-SX, Yamaha FGDP
- Calibration wizard, histograms, A/B compare, note remap
- Humanize, library compensation, zone routing by pad group
- Standalone app with MIDI in/out

## Known limitations

| Area | What to expect |
|------|----------------|
| **Quality** | v0.2.x — UI and edge cases still improving; report issues on GitHub |
| **macOS installs** | Release zips are **unsigned**; one-time allow or `xattr -cr` (see [install](docs/user/install.md)) |
| **Ableton** | No third-party MIDI FX slot — use the **VST3 Instrument** build |
| **MIDI 2.0** | High-res LUT built in; **host UMP I/O** not wired yet — most setups use MIDI 1.0 |
| **Host coverage** | pluginval checks load/stability; full DAW smoke on every host is not complete |

## Contributing

Developers: [docs/README.md](docs/README.md) · [CONTRIBUTING.md](docs/developer/CONTRIBUTING.md).

## License

MIT — see [LICENSE](LICENSE).

---

**Version & changelog:** [CHANGELOG.md](CHANGELOG.md) · [Versioning policy](docs/developer/VERSIONING.md)

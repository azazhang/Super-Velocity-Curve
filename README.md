# Super VelocityCurve

Free, open-source **MIDI velocity curves** built for finger drummers and pad players — not keyboard players tweaking one global curve.

Shape how hard you hit maps to MIDI velocity **per pad**, with **controller-specific profiles** that travel with you across DAWs and drum plugins.

> **Status: beta (v0.2.x)** — core workflow works; expect rough edges. Known limitations are listed [below](#known-limitations).

[![Verified by pluginval](https://assets.tracktion.com/img/pages/develop/develop-logo-pluginval.png)](https://www.tracktion.com/develop/pluginval) — automated validation at strictness 5 (VST3/AU in CI).

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

**A/B compare**, **live histograms**, **calibration wizard**, and **note remap** round out the workflow.

## Download & install

**→ [Install guide](docs/user/install.md)** — copy plugins into the right folders, rescan, troubleshoot unsigned builds.

**GitHub Releases:** [github.com/azazhang/Super_VelocityCurve/releases](https://github.com/azazhang/Super_VelocityCurve/releases)  
If the page is empty, no release zip has been published yet — you can [build from source](docs/developer/CONTRIBUTING.md).

## Getting started

**→ [Getting started](docs/user/getting-started.md)** — per-DAW routing, curve editor, profiles, save/export.

## Which plugin do I need?

Two builds ship in each release: **Instrument** (Ableton) and **MIDI FX** (most other DAWs). On macOS you also get **AU** and **CLAP** (MIDI FX only).

| DAW | Plugin name | Formats | Where to load |
|-----|-------------|---------|---------------|
| Ableton Live | Super VelocityCurve | VST3 Instrument | MIDI track (instrument slot — not MIDI FX) |
| Logic Pro | Super VelocityCurve MIDI FX | AU | MIDI FX slot above the instrument |
| Reaper | Super VelocityCurve MIDI FX | VST3, CLAP; AU on macOS | Track input FX (before instrument) |
| Bitwig | Super VelocityCurve MIDI FX | CLAP (preferred), VST3 | Note FX chain |
| FL Studio | Super VelocityCurve MIDI FX | VST3 | MIDI effect on channel (before sampler) |
| Cubase / Nuendo | Super VelocityCurve MIDI FX | VST3 | MIDI Modifiers / MIDI plugin slot |
| Studio One | Super VelocityCurve MIDI FX | VST3 | Event FX / MIDI FX (version-dependent) |
| Standalone | Super VelocityCurve | app / exe | Route via IAC (Mac) or loopMIDI (Windows) |

Details and screenshots-level steps: [getting-started.md](docs/user/getting-started.md).

## Features

- Per-pad velocity curves with gate handles, presets, floor/ceiling
- Factory profiles: GM, Launchpad 8×8, Maschine, SPD-SX, Yamaha FGDP
- Calibration wizard, histograms, A/B compare, note remap
- Humanize, library compensation, zone routing by pad group
- Standalone app with MIDI in/out

## Known limitations

This is honest beta software — not a finished 1.0 product.

| Area | What to expect |
|------|----------------|
| **Quality** | v0.2.x — UI and edge cases still improving; report issues on GitHub |
| **macOS installs** | Release zips are **unsigned**; you may need a one-time security allow or `xattr -cr` (see [install](docs/user/install.md)) |
| **Ableton** | No third-party MIDI FX slot — use the **Instrument** build and route MIDI to your drum track |
| **CLAP** | MIDI FX only — there is no CLAP Instrument build for Ableton-style routing |
| **MIDI 2.0** | High-res output LUT is built in; **host UMP I/O** is not wired yet — most setups still use MIDI 1.0 |
| **Releases** | Automated CI builds plugins; **GitHub release zips** appear when a maintainer publishes a version tag |
| **Host coverage** | pluginval checks load/stability; **full DAW smoke** on every host is not complete — try your setup and open an issue if something fails |

## Support the project

If Super VelocityCurve helps your playing or teaching:

- [Buy Me a Coffee](https://buymeacoffee.com/azhang)
- [Ko-fi — Studio J](https://ko-fi.com/studioj)

## Contributing

Developers: [docs/README.md](docs/README.md) (build, test, CI).  
Contributors welcome — see [CONTRIBUTING.md](docs/developer/CONTRIBUTING.md).

## License

MIT — see [LICENSE](LICENSE).

---

**Version & changelog:** [CHANGELOG.md](CHANGELOG.md) · [Versioning policy](docs/developer/VERSIONING.md) (beta numbering)

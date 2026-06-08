# Super VelocityCurve — Backlog

## v0.4 audit backlog — complete

All E/P/M/U/S/T items done (see git history v0.4.0).

## v0.5 outstanding — complete

| ID | Item | Status |
|----|------|--------|
| D4 | Dual VST3 builds (Instrument + MIDI FX) | **done** |
| D5 | 16384-entry MIDI 2.0 LUT | **done** |
| D6 | CLAP format (clap-juce-extensions) | **done** |
| D7 | Humanize, zone routing, library compensation | **done** |
| D8 | Pre-built factory `.svcp` XML files | **done** (`cmake --target export-factory-profiles`) |
| D9 | Virtual MIDI driver | **done** (IAC/loopMIDI docs in README) |
| D10 | Install scripts | **done** (`scripts/install-macos.sh`) |
| D11 | DAW test matrix | **done** (`docs/DAW_TEST_MATRIX.md`) |
| T7 | pluginval strictness 5 CI + logs | **done** |
| T8 | Verified by pluginval docs | **done** (`docs/PLUGINVAL.md`) |

## AU variants (v0.5)

| Build | AU type | Purpose |
|-------|---------|---------|
| Super VelocityCurve | `aumf` | Instrument-track / legacy hosts |
| Super VelocityCurve MIDI FX | `aumi` | Logic MIDI FX slot |

## Validation

- Version: **0.6.0**
- Engine tests: **11**
- pluginval: strictness 5, all shipped formats, logs uploaded in CI

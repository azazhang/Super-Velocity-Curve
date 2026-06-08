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
| D8 | Pre-built factory `.svcp` XML files | **done** |
| D9 | Virtual MIDI driver | **done** (IAC/loopMIDI docs) |
| D10 | Install scripts | **done** |
| D11 | DAW test matrix | **done** |
| T7 | pluginval strictness 5 CI + logs | **done** (Windows CLAP exception — see PLUGINVAL.md) |
| T8 | Verified by pluginval docs + in-plugin badge | **done** |

## QA backlog (2026-06-08)

From test pyramid: `ctest` pass (12 engine tests), pluginval local skip (binary not installed), static smoke review. See [docs/developer/TESTING.md](docs/developer/TESTING.md).

### Bug list

| ID | Severity | Summary | Status |
|----|----------|---------|--------|
| BUG-001 | major | A/B audition overwritten when editing during “Hearing A/B” | **fixed** — `syncAbAuditionIfActive()` after engine sync |
| BUG-002 | major | Pad inspector edits cleared live hit flashes | **fixed** — `updatePad` + `setPad(..., false)` |
| BUG-003 | major | Duplicate MIDI note+channel on multiple pads — engine last-wins, UI first-match | **partial** — blocked on add/edit; import still unvalidated |
| BUG-004 | minor | Preset toolbar cramped at min width 1100×760 | open |
| BUG-005 | minor | Title/subtitle painted in `paint()` but labels not laid out | open |
| BUG-006 | minor | Unused `curveB` member | **fixed** |

### P0 — Correctness / release gate

- [x] **P0** A/B audition survives profile/engine sync while compare active
- [ ] **P0** Run local pluginval before tag (`pluginval` v1.0.4 + `./scripts/validate-plugins-local.sh`)
- [ ] **P0** Windows CLAP: add `clap-info` CI smoke OR stop shipping Windows CLAP (pluginval literal gap — [PLUGINVAL.md](docs/developer/PLUGINVAL.md))

### P1 — Smoke matrix / UX

- [x] **P1** Preserve hit markers when editing pad settings (non-curve fields)
- [ ] **P1** Warn on duplicate `(midiNote, midiChannel)` across pads
- [ ] **P1** Manual DAW smoke: Ableton Instrument + Logic MIDI FX + min window size
- [x] **P1** About dialog + pluginval badge (v0.6.2)
- [x] **P1** Add/delete pads + save-in-place for user profiles (v0.6.2)

### P2 — Polish / tests

- [ ] **P2** Layout pass at 1100×760 for curve preset button row
- [x] **P2** Remove dead `curveB` state
- [ ] **P2** Clarify Hear A/B button labels
- [x] **P2** Engine test: duplicate note/channel pad mapping policy (profile layer)
- [ ] **P2** MIDI replay tests for deterministic velocity output

## AU variants (v0.5)

| Build | AU type | Purpose |
|-------|---------|---------|
| Super VelocityCurve | `aumf` | Instrument-track / legacy hosts |
| Super VelocityCurve MIDI FX | `aumi` | Logic MIDI FX slot |

## Validation

- Version: **0.6.2**
- Engine tests: **12**
- pluginval: strictness 5 on all formats pluginval can load; Windows CLAP exception documented

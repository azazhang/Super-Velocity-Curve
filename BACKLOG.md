# Super VelocityCurve — Backlog

## Honest status (2026-06-10)

**v0.2.8:** Windows CI segfault fix — heap-backed `HistogramBank`, layout-test GUI init. **v0.2.7:** Pad label layout, live hits linger, AT curve toggle, calibration wizard, Ko-fi link, user README/docs cleanup. **v0.2.6:** UI fixes — pad scrollbar, idle repaint, MIDI note names, save-dialog, profile-switch perf. **v0.2.5:** Engine QA fixes — zone-routing note-off/aftertouch, gated note-off suppression, lock-free retrigger, histogram thread safety. **v0.2.4:** pluginval state crash fixed. **v0.2.3:** Layout/UI automated tests; CI-3 universal macOS release.

**Still requires human verification:** P1 DAW smoke (Ableton Instrument + Logic MIDI FX). Run `./scripts/qa-iterate.sh --pluginval` then manual checklist in `docs/developer/TESTING.md`.

**Deferred:** CI-4/CI-5 shared static lib refactors; learn-pad-from-hit; host UMP I/O wiring.

## P0 — Correctness / release gate

- [x] P0 Engine tests (**23**)
- [x] P0 Layout/UI tests (**12** cases in `LayoutTests`)
- [x] P0 `scripts/validate-plugins-local.sh` + `scripts/validate-clap.sh`
- [x] P0 CLAP `clap-validator` in CI

## P1 — Smoke matrix / UX

- [ ] **P1 Manual DAW smoke** — Ableton Instrument + Logic MIDI FX (`docs/developer/DAW_TEST_MATRIX.md`)

## CI backlog

| ID | Item | Status |
|----|------|--------|
| CI-1–CI-2, CI-6, CI-7 | macOS speed, pluginval, concurrency, tiered CI | **done** |
| CI-3 | Universal macOS via parallel `arm64` + `x86_64` + `lipo` | **done** v0.2.3 (`release.yml`) |
| CI-4 / CI-5 | Shared compile unit across dual plugins | **deferred** |

## Validation

- Version: **0.2.8** (`CMakeLists.txt`)
- Tests: **23** engine + **12** layout (`ctest`)
- Local QA: `./scripts/qa-iterate.sh [--pluginval]`

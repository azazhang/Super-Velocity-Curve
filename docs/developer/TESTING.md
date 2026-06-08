# Testing & QA strategy

Super VelocityCurve cannot rely on a single human tester. This document describes the layered approach we use and plan to expand.

## Test pyramid

```
                    ┌─────────────────┐
                    │  Manual DAW     │  You + beta users — routing, feel, hosts
                    │  smoke matrix   │
              ┌─────┴─────────────────┴─────┐
              │  pluginval strictness 5     │  CI — plugin load, buses, editor, stability
        ┌─────┴─────────────────────────────┴─────┐
        │  Engine unit tests (ctest)              │  Curves, gates, remap, MIDI 2 LUT
  ┌─────┴─────────────────────────────────────────┴─────┐
  │  Scripted MIDI replay (planned)                     │  Deterministic note sequences → expected velocities
  └─────────────────────────────────────────────────────┘
```

## What runs automatically today

| Layer | Command / trigger | Covers |
|-------|-------------------|--------|
| Engine tests | `ctest --test-dir build --output-on-failure` | Monotonic LUT, retrigger, gates, remap, profile round-trip |
| pluginval | GitHub Actions `build.yml` | VST3/AU load, in-process validation, strictness 5 |
| Local pluginval | `./scripts/validate-plugins-local.sh` | Same as CI, bounded timeout |

Run engine tests on every push. pluginval runs on macOS (all formats) and Windows (VST3; CLAP is build-only on Windows — see [PLUGINVAL.md](PLUGINVAL.md)).

## Manual smoke matrix (release gate)

Before tagging a release, walk [DAW_TEST_MATRIX.md](DAW_TEST_MATRIX.md) for at least:

1. **Pad selection** — click every quadrant of the pad grid; curve title updates.
2. **Copy / paste** — copy curve on pad A, select pad B, paste; curve changes.
3. **Capture A / Hear A-B** — capture, edit curve, toggle hear; status and curve overlay change.
4. **Pad edit** — rename pad, change MIDI note; play note and confirm histogram hits correct pad.
5. **Profile** — save, duplicate, export `.svcp`, import, delete.
6. **Routing tab** — channel filter; **Note Remap** tab — add remap row.
7. **Themes** — dark and light; no overlapping labels.

Record host, OS, and plugin variant in the release checklist.

## Bug-finding practices (industry standard for audio plugins)

1. **Automate the deterministic core** — velocity math, gates, remap, and profile XML must stay in `Tests/EngineTests.cpp`. Add a test for every bug fix when possible.
2. **CI pluginval** — catches format/host API mistakes humans miss; keep strictness 5.
3. **Bounded local validation** — never run unbounded pluginval/lldb in scripts (see `scripts/validate-plugins-local.sh`).
4. **Checklist-driven manual QA** — humans excel at feel and DAW-specific routing; use the matrix above, not ad-hoc clicking.
5. **Beta releases** — GitHub pre-releases with unsigned zips; gather host/OS reports before `latest`.
6. **Structured bug reports** — steps, expected vs actual, profile `.svcp`, plugin variant (Instrument vs MIDI FX).

### Planned improvements

| Item | Benefit |
|------|---------|
| MIDI replay tests | Feed fixed `MidiBuffer` sequences; assert output velocities without UI |
| JUCE `UnitTest` for profile/pad edits | Regression-test pad rename + note change logic |
| Headless editor smoke (optional) | Instantiate editor off-message-thread where hosts allow |
| Crash telemetry (opt-in) | Field crashes on unsupported hosts |

## Adding a test for a bug fix

1. Reproduce with the smallest engine-level case (preferred) or document in the smoke matrix if UI-only.
2. Add `static int testYourCase()` in `Tests/EngineTests.cpp`.
3. Register in `main()` and run `ctest`.

## Agent / automation rules

- Do **not** block chat on `gh run watch`; spawn a subagent or use `./scripts/watch-ci.sh` with timeout.
- Do **not** run pluginval without `--validate-in-process` and a hard timeout on Windows.
- Prefer fixing root cause + engine test over one-off UI hacks.

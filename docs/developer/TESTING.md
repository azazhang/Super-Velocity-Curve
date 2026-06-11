# Testing & QA strategy

Super Velocity Curve cannot rely on a single human tester. This document describes the layered approach we use and plan to expand.

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
| Engine tests | `ctest --test-dir build --output-on-failure` | Monotonic LUT, retrigger, gates, remap, profile round-trip, MIDI 2 transport |
| Layout/UI tests | `ctest` (`LayoutTests`) | Min-window curve bounds, pad merge, pad-grid scroll, headless editor |
| pluginval | GitHub Actions `build.yml` | VST3/AU load, in-process validation, strictness 5 |
| Local pluginval | `./scripts/validate-plugins-local.sh` | Same as CI, bounded timeout |
| QA iteration script | `./scripts/qa-iterate.sh [--pluginval]` | Build + ctest + checklist reminder |

Run engine tests on every push. pluginval runs on macOS and Windows (VST3/AU). CLAP uses **clap-validator** on both platforms — see [PLUGINVAL.md](PLUGINVAL.md).

## Manual smoke matrix (release gate)

Before tagging a release, walk [DAW_TEST_MATRIX.md](DAW_TEST_MATRIX.md) for at least:

1. **Pad selection** — click every quadrant of the pad grid; curve title updates.
2. **Pad switch persistence** — edit curve on pad A, switch to pad B and back; pad A curve must match what you left (no silent overwrite from inspector stale state).
3. **Minimum window layout** — resize to default/minimum height; velocity curve plot must remain clearly visible (not ~0px).
4. **Live hit feedback** — play mapped pad; curve shows crosshair + `in→out` label on latest hit; pad grid flashes; live hits label updates.
5. **Copy / paste** — copy curve on pad A, select pad B, paste; curve changes.
6. **Capture A / Hear A-B** — capture, edit curve, toggle hear; status and curve overlay change.
7. **Pad edit** — rename pad, change MIDI note; play note and confirm histogram hits correct pad.
8. **Profile** — save, duplicate, export `.svcp`, import, delete.
9. **Routing tab** — channel filter; **Note Remap** tab — add remap row.
10. **Themes** — dark and light; no overlapping labels.
11. **Pad grid scroll** — with many pads, horizontal scroll works; no clipped headers.

Record host, OS, and plugin variant in the release checklist.

### Agent / developer pre-ship gate (mandatory)

Do **not** mark a UI fix “done” or update `BACKLOG.md` to resolved until **all** of the following pass on a **fresh local build**:

| Gate | Why it failed us |
|------|------------------|
| `cmake --build build` + `ctest` green | Claimed fixes without a verified binary |
| Open Standalone at default size | Missed invisible curve (layout starvation) |
| Pad switch round-trip after curve edit | Missed inspector overwriting curve editor state |
| Play MIDI into mapped pad | Missed weak/absent hit visualization |
| Scroll pad grid with 16+ pads | Missed horizontal scroll regression |

If only engine logic changed, gates 2–4 can be shortened; if `PluginEditor`, layout, or inspector changed, run the full list.

## Postmortem: why critical UI bugs shipped (v0.6.5–0.6.7)

| Bug | Symptom | Root cause in QA process |
|-----|---------|--------------------------|
| Invisible curve | Editor looked broken | No layout test at minimum window size; flex weights starved the curve |
| Curve lost on pad switch | Edits vanished | No pad-switch persistence scenario; two UI sources (curve editor vs inspector) not merged in tests |
| Sluggish drag | UI felt broken | No interaction perf check; unconditional 20Hz full repaints + per-frame LUT rebuild |
| False “fixed” backlog | User could not verify | Status updated from code review, not from rebuilt app + smoke checklist |

**Lesson:** Automated tests covered velocity math, not **layout**, **multi-widget state**, or **interaction feel**. Humans skipped structured smoke steps. Agents declared completion without opening the built Standalone.

## Bug-finding practices (industry standard for audio plugins)

1. **Automate the deterministic core** — velocity math, gates, remap, and profile XML must stay in `Tests/EngineTests.cpp`. Add a test for every bug fix when possible.
2. **Shift-left UI regressions** — layout bounds, pad-switch state merge, and scroll visibility belong in the release checklist (above), not “we’ll notice when testing.”
3. **Single source of truth per field** — when two widgets edit the same pad, document merge order (`mergeActivePadFromUI`) and test pad-switch round-trips manually.
4. **CI pluginval** — catches format/host API mistakes humans miss; keep strictness 5.
5. **Bounded local validation** — never run unbounded pluginval/lldb in scripts (see `scripts/validate-plugins-local.sh`).
6. **Checklist-driven manual QA** — humans excel at feel and DAW-specific routing; use the matrix above, not ad-hoc clicking.
7. **Definition of Done** — fix + test (if engine) + **rebuilt binary** + relevant smoke rows; never close P0 from diff review alone.
8. **Beta releases** — GitHub pre-releases with unsigned zips; gather host/OS reports before `latest`.
9. **Structured bug reports** — steps, expected vs actual, profile `.svcp`, plugin variant (Instrument vs MIDI FX).

### Planned improvements

| Item | Benefit |
|------|---------|
| MIDI replay tests | Feed fixed `MidiBuffer` sequences; assert output velocities without UI |
| JUCE `UnitTest` for profile/pad edits | Regression-test pad rename + note change logic |
| ~~Layout bounds unit test~~ | **Done** v0.2.3 — `Tests/LayoutTests.cpp` + `EditorLayout` |
| Headless editor smoke (optional) | Instantiate editor off-message-thread where hosts allow |
| Crash telemetry (opt-in) | Field crashes on unsupported hosts |

## Adding a test for a bug fix

1. Reproduce with the smallest engine-level case (preferred) or document in the smoke matrix if UI-only.
2. Add `static int testYourCase()` in `Tests/EngineTests.cpp`.
3. Register in `main()` and run `ctest`.

## Agentic QA (interactive validation)

Scripted `ctest` cannot catch timer flicker, modal misfires, truncated pad labels, or profile-combo side effects. **Coding agents must run scenario packs** in [AGENTIC_QA.md](AGENTIC_QA.md) on a fresh Standalone build before marking UI work done.

Quick rule: if you changed `PluginEditor` or any `Source/UI/*` file, run **Pack A** plus the packs listed in the file→pack table in AGENTIC_QA.md.

## Agent / automation rules

- Do **not** block chat on `gh run watch`; spawn a subagent or use `./scripts/watch-ci.sh` with timeout.
- Do **not** run pluginval without `--validate-in-process` and a hard timeout on Windows.
- Prefer fixing root cause + engine test over one-off UI hacks.
- **Do not** claim UI fixes verified without `qa-iterate.sh` + relevant AGENTIC_QA packs.

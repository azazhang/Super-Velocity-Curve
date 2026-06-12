# Agent instructions (Super Velocity Curve)

This file is for **AI coding agents** and contributors only — not linked from the top of [README.md](README.md). End users: [README.md](README.md) → [docs/user/](docs/user/). Doc map: [docs/README.md](docs/README.md).

## Repository map

- **JUCE plugin** with dual targets: Instrument (`SuperVelocityCurve`) + MIDI FX (`SuperVelocityCurveMidiFx`)
- **Engine** is DAW-agnostic; plugin wrappers are thin
- **Profiles** serialize to XML `.svcp` via `ProfileStore`
- **Theme** in `Source/UI/Theme.h` — `ThemeMode::dark` (default) and `light`
- **URLs** in `Source/Config/AppUrls.h` — GitHub, homepage, YouTube, support, pluginval
- **Naming:** **Super Velocity Curve** in UI/copy; **SuperVelocityCurve** for repo, code, and release zips. Run `./scripts/check-branding.sh` before commit.

## Build & test

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

### Mandatory: rebuild after every code change

**After any edit to `Source/`, `Tests/`, or `CMakeLists.txt`, you MUST:**

1. Run `cmake --build build` (full plugin targets, not tests-only).
2. Run `ctest --test-dir build --output-on-failure`.
3. Tell the user the **exact path and binary timestamp** to open, e.g.  
   `build/SuperVelocityCurve_artefacts/RelWithDebInfo/Standalone/Super Velocity Curve.app`  
   Do not end a fix session with “rebuild yourself” or only `open` instructions without having built first.

Local `COPY_PLUGIN_AFTER_BUILD` also installs VST3/AU/CLAP to `~/Library/Audio/Plug-Ins/` — mention that if relevant.

Never `git add build/` or `build-ci-test/`. Never commit without explicit user request.

## CI rules

- **Spawn a background subagent** to monitor CI (`gh run view` polling / `scripts/watch-ci.sh`). Do **not** block the main agent on `gh run watch` or unbounded polls.
- Use subagents whenever tasks are **parallelizable and independent** (CI watch, test pyramid, doc research, log triage).
- Windows pluginval paths with spaces must use quoted `Start-Process` argument strings
- `COPY_PLUGIN_AFTER_BUILD=OFF` on CI
- Windows CLAP is not pluginval-testable (flat DLL); see [docs/developer/PLUGINVAL.md](docs/developer/PLUGINVAL.md)

## Docs split

| Audience | Location |
|----------|----------|
| Musicians | `README.md`, `docs/user/` |
| Developers | `docs/developer/`, `CHANGELOG.md`, `BACKLOG.md` |
| Agents | This file |

Do not put agent/session memory in README.md.

## Ableton constraint

Ableton requires **Instrument** VST3 with audio bus. MIDI-FX-capable DAWs use the separate **MIDI FX** target. Do not merge into one binary.

## QA

Follow [docs/developer/TESTING.md](docs/developer/TESTING.md). Add engine tests for deterministic bugs; update `BACKLOG.md` with P0/P1/P2 items.

**Iterative QA loop:** `./scripts/qa-iterate.sh [--pluginval]` → fix failures → re-run until green → manual smoke checklist.

**Never mark a bug fixed without:**

1. Rebuilding (`cmake --build build`) and running `ctest`.
2. Running the **pre-ship gate** rows in TESTING.md that apply to your change (layout / pad-switch / live hits for UI work).
3. Reporting the Standalone `.app` path and binary timestamp to the user.

Engine-only fixes need (1) + engine test when feasible. UI/layout/state fixes need (1)–(3) including manual smoke, not just code review.

## Current version

See `CMakeLists.txt` `project(VERSION …)` and `CHANGELOG.md`.

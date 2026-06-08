# Agent instructions (Super VelocityCurve)

This file is for **AI coding agents** and contributors. End users should read [README.md](README.md) and [docs/user/](docs/user/).

## Repository map

- **JUCE plugin** with dual targets: Instrument (`SuperVelocityCurve`) + MIDI FX (`SuperVelocityCurveMidiFx`)
- **Engine** is DAW-agnostic; plugin wrappers are thin
- **Profiles** serialize to XML `.svcp` via `ProfileStore`
- **Theme** in `Source/UI/Theme.h` — `ThemeMode::dark` (default) and `light`
- **URLs** in `Source/Config/AppUrls.h` — GitHub, homepage, YouTube, support, pluginval

## Build & test

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSVC_COPY_PLUGIN_AFTER_BUILD=OFF
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

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

## Current version

See `CMakeLists.txt` `project(VERSION …)` and `CHANGELOG.md`.

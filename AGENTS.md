# Agent instructions (Super VelocityCurve)

This file is for **AI coding agents** and contributors. End users should read [README.md](README.md) and [docs/user/](docs/user/).

## Repository map

- **JUCE plugin** with dual targets: Instrument (`SuperVelocityCurve`) + MIDI FX (`SuperVelocityCurveMidiFx`)
- **Engine** is DAW-agnostic; plugin wrappers are thin
- **Profiles** serialize to XML `.svcp` via `ProfileStore`
- **Theme** in `Source/UI/Theme.h` — `ThemeMode::dark` (default) and `light`

## Build & test

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSVC_COPY_PLUGIN_AFTER_BUILD=OFF
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Never `git add build/` or `build-ci-test/`. Never commit without explicit user request.

## CI rules

- Do **not** use unbounded `gh run watch` in background shells — use `scripts/watch-ci.sh` (45 min cap)
- Windows pluginval paths with spaces must use quoted `Start-Process` argument strings
- `COPY_PLUGIN_AFTER_BUILD=OFF` on CI

## Docs split

| Audience | Location |
|----------|----------|
| Musicians | `README.md`, `docs/user/` |
| Developers | `docs/developer/`, `CHANGELOG.md` |
| Agents | This file |

Do not put agent/session memory in README.md.

## Ableton constraint

Ableton requires **Instrument** VST3 with audio bus. MIDI-FX-capable DAWs use the separate **MIDI FX** target. Do not merge into one binary.

## Current version

See `CMakeLists.txt` `project(VERSION …)` and `CHANGELOG.md`.

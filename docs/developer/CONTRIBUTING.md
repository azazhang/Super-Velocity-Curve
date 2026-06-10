# Contributing

## Build

```bash
git submodule update --init --recursive
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
```

### Build output folders (why so many?)

JUCE emits **one artefacts folder per CMake target**, not one folder for the whole repo:

| Folder | Target |
|--------|--------|
| `build/SuperVelocityCurve_artefacts/` | Instrument plugin (VST3, AU, Standalone) |
| `build/SuperVelocityCurveMidiFx_artefacts/` | MIDI FX plugin (+ CLAP) |
| `build/SuperVelocityCurveTests_artefacts/` | `ctest` console binary |

Inside each, **`RelWithDebInfo/` vs `Release/`** is the CMake build type. They coexist if you reconfigure between types. **Use one type consistently** — CI uses `RelWithDebInfo`; local dev can match with `-DCMAKE_BUILD_TYPE=RelWithDebInfo`.

**Run the app:** `build/SuperVelocityCurve_artefacts/RelWithDebInfo/Standalone/Super VelocityCurve.app`

To avoid stale duplicates: `rm -rf build && cmake -B build ...` when switching build types.

macOS universal CI build:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DSVC_COPY_PLUGIN_AFTER_BUILD=OFF
```

## Project layout

| Path | Purpose |
|------|---------|
| `Source/Engine/` | Velocity curve, MIDI processing |
| `Source/Profiles/` | Controller profiles, `.svcp` serialization |
| `Source/Plugin/` | JUCE processor + editor |
| `Source/UI/` | Theme, curve editor, pad grid |
| `Tests/` | Engine unit tests |
| `docs/user/` | Musician-facing docs (linked from README) |
| `docs/developer/` | CI, DAW matrix, pluginval |
| `AGENTS.md` | Instructions for AI coding agents |

## CI

Push to `main` runs `.github/workflows/build.yml` (macOS + Windows, pluginval strictness 5 on VST3/AU, clap-validator on CLAP).

No Windows machine? See [WINDOWS_TESTING.md](WINDOWS_TESTING.md) — download CI artifacts or use a VM.

Tag `v*.*.*` runs `.github/workflows/release.yml` and publishes GitHub Release artifacts.

CI uses **RelWithDebInfo + no LTO + Ninja + ccache** on macOS (see [CI.md](CI.md)). Local Release builds still use LTO by default.

## Versioning

- Bump `project(VERSION …)` in `CMakeLists.txt`
- Add entry to `CHANGELOG.md`
- Tag `vX.Y.Z` after CI is green

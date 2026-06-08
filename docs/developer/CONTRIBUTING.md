# Contributing

## Build

```bash
git submodule update --init --recursive
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

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

Push to `main` runs `.github/workflows/build.yml` (macOS + Windows, pluginval strictness 5).

Tag `v*.*.*` runs `.github/workflows/release.yml` and publishes GitHub Release artifacts.

## Versioning

- Bump `project(VERSION …)` in `CMakeLists.txt`
- Add entry to `CHANGELOG.md`
- Tag `vX.Y.Z` after CI is green

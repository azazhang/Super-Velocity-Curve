# CI architecture

## Root cause (2026-06-08 audit)

**macOS has never failed on pluginval in the v0.5+ era.** Every red macOS job died in **`cmake --build`** after ~60–90 minutes and hit the **90-minute job timeout** before tests or pluginval ran.

**Windows failures (v0.5–v0.6.0)** were from running **pluginval on CLAP** (pluginval does not support CLAP; flat Windows `.clap` DLLs are valid). Fixed in v0.2.2: **clap-validator** for CLAP on macOS and Windows.

### Why macOS compile takes ~90 minutes

| Factor | Impact |
|--------|--------|
| **Dual JUCE plugins** (Instrument + MIDI FX) | Two full static shared-code libs; all `SVC_PLUGIN_SOURCES` + JUCE modules compiled **twice** |
| **Release + LTO** (`juce_recommended_lto_flags` → `-flto`) | Apple Clang LTO link of large JUCE archives is extremely slow |
| **Six macOS plugin bundles** | Instrument AU+VST3+Standalone + MIDI FX AU+VST3+CLAP |
| **No compiler cache** | Cold compile every push |
| **Default generator** | Makefiles instead of Ninja |
| **Stacked pushes** | New commits start new runs; old macOS jobs burn 90m then cancel |

Windows completes build in ~7–10m because MSVC LTCG is faster here and there is no AU format.

### What we changed (structural, not band-aids)

1. **`SVC_ENABLE_LTO=OFF` in CI** — LTO kept for local Release builds; disabled on `ENV{CI}`.
2. **Ninja + ccache** — faster builds and warm-cache hits on repeat pushes.
3. **`RelWithDebInfo` in CI** — full optimization without LTO link cost; pluginval-compatible.
4. **`concurrency: cancel-in-progress`** — one active build per branch; no stacked 90m jobs.
5. **Release workflow: universal via parallel arch + lipo** (CI-3, v0.2.3) — see [Intel Mac / universal builds](#intel-mac-universal-builds).

## Intel Mac / universal builds

**There is no separate “Intel Mac CI job” failing today.** CI builds **`arm64` only** (`CMAKE_OSX_ARCHITECTURES=arm64`) and completes in ~10–25 minutes.

The **90-minute timeout** came from attempting a **universal fat binary in a single job**:

```bash
-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
```

That forces **every translation unit to compile twice** (once per arch) and links **fat binaries** for six plugin bundles × two JUCE targets. With **Release + LTO** (pre-v0.2 CI), link alone could exceed an hour. arm64-only is ~10× faster — not because Intel is broken, but because universal-in-one-job duplicates the entire compile+link graph.

| Approach | Typical macOS CI time | Ships Intel Mac? |
|----------|----------------------|------------------|
| `arm64` only (current) | ~10–25 min | Rosetta in hosts |
| `arm64;x86_64` one job | **>90 min** (timeout) | Yes (fat) |
| **CI-3:** parallel `arm64` + `x86_64` jobs + `lipo` merge | ~15 min per arch (parallel) | **Yes** (`release.yml`) |

**CI-3 plan:** two matrix jobs, each single-arch + ccache, then a small `lipo` packaging job — never compile both arches in one CMake configure.

**Still open:** shared compile unit for engine/UI across dual plugins (**CI-4/CI-5**) — needs JUCE-compatible OBJECT/static target wiring.

### Expected timings (after fix)

| Job | Target |
|-----|--------|
| macOS `build.yml` | &lt;25 min cold, &lt;12 min warm (ccache) |
| Windows `build.yml` | ~10 min (unchanged) |
| Release tag builds | &lt;30 min per arch job |

### Green build history

Last full green before v0.5 dual-plugin era: **v0.4.2** (~16 min macOS). v0.5+ added dual plugins + CLAP + LTO duplication → macOS timeout regression.

Windows **green** on run 27174278645 (~9 min). macOS **compile** ~2m after LTO fix. pluginval: VST3+AU pass with Components install; **CLAP** needs 420s timeout (180s alarm was too short on CI). Historical: run 27171968644 AU hung 42m before per-plugin `perl alarm`.

## Local parity

```bash
cmake -B build-ci -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DSVC_ENABLE_LTO=OFF \
  -DSVC_COPY_PLUGIN_AFTER_BUILD=OFF
cmake --build build-ci --parallel
ctest --test-dir build-ci --output-on-failure
```

## Backlog

See [BACKLOG.md](../../BACKLOG.md) section **CI backlog**.

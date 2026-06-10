# Testing Windows builds without a Windows PC

## Recommended: CI artifacts

Every push to `main` runs `.github/workflows/build.yml` on `windows-latest` (build + ctest + pluginval VST3 + clap-validator CLAP).

1. Open [Actions](https://github.com/azazhang/Super_VelocityCurve/actions) → latest green **Build** workflow.
2. Download artifact **`SuperVelocityCurve-Windows`** (`SuperVelocityCurve-Windows-unsigned.zip`).
3. Unzip — contains VST3, Standalone `.exe`, and CLAP `.clap` DLL.
4. Copy into host folders per [install guide](../user/install.md) on a Windows machine or VM.

This is the primary gate; you do not need local Windows hardware to know the build passed CI.

## Windows VM on Apple Silicon Mac

- **Parallels** or **UTM** with Windows 11 ARM — good enough for Reaper/Bitwig smoke tests.
- Install the unsigned zip; allow blocked plugins per DAW docs.
- Use **loopMIDI** for Standalone MIDI routing tests.

## Remote / cloud Windows

- GitHub Actions `workflow_dispatch` on **Build** re-runs the full Windows job on demand.
- Optional: rent an hourly Windows cloud desktop only for manual DAW smoke (P1 backlog).

## What does *not* work well

| Approach | Why skip |
|----------|----------|
| **Wine** on macOS | DAWs and pluginval are unreliable; not a release gate |
| **Cross-compile Windows from Mac** | MSVC toolchain is Windows-native; we use CI instead |
| **pluginval for CLAP on Windows** | pluginval does not validate CLAP; use **clap-validator** (already in CI) |

## Local validation parity (macOS host)

You cannot run the Windows `.exe` natively on macOS. Validate logic via:

```bash
ctest --test-dir build --output-on-failure   # engine tests (cross-platform)
./scripts/validate-plugins-local.sh        # macOS VST3/AU pluginval
./scripts/validate-clap.sh                 # macOS CLAP clap-validator
```

Trust Windows-specific packaging from CI artifacts + logs (`pluginval-logs-windows`).

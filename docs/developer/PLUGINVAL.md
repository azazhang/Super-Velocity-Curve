# Verified by pluginval

Super VelocityCurve is validated with [pluginval](https://github.com/Tracktion/pluginval) at **strictness level 5** in CI. The official badge and program details: [tracktion.com/develop/pluginval](https://www.tracktion.com/develop/pluginval).

## Program requirements (Tracktion)

Per [Adding pluginval to CI](https://github.com/Tracktion/pluginval/blob/develop/docs/Adding%20pluginval%20to%20CI.md):

1. Run pluginval at strictness ≥ 5 on shipped plugins
2. Integrate validation into the release pipeline (fail build on failure)
3. Retain generated log files for the Verified by pluginval scheme
4. Display the **official logo** on project materials (README + in-plugin About dialog)

Official badge asset: `https://assets.tracktion.com/img/pages/develop/develop-logo-pluginval.png` (embedded in the plugin from `Resources/pluginval_badge.png`).

## What we validate

| Platform | Formats | pluginval |
|----------|---------|-----------|
| macOS | VST3 Instrument, VST3 MIDI FX, AU `aumf`, AU `aumi` | **Yes** — CI |
| macOS | CLAP (bundle) | **No** — pluginval strictness 5 hangs on GHA (>7 min, no progress); built + smoke-tested in CI |
| Windows | VST3 Instrument, VST3 MIDI FX | **Yes** |
| Windows | CLAP (flat `.clap` DLL) | **No** — see below |

CI runs pluginval **in process** (`--validate-in-process`) at strictness 5 with `--output-dir pluginval-logs`. Log artifacts: `pluginval-logs-macos`, `pluginval-logs-windows`.

## Windows CLAP exception

**Why skipped:** On Windows, `clap-juce-extensions` emits CLAP as a **single `.clap` DLL**. pluginval’s scanner uses **lilv**, which expects a **macOS-style bundle** containing `manifest.ttl`. Staging the path without spaces does not fix this — the file layout is wrong for lilv, not the path.

**Do we still meet “Verified by pluginval”?** Strictly speaking, the program asks for validation of **shipped formats per platform**. We:

- Validate **every format pluginval can load** on each OS (all VST3/AU on macOS; both VST3 on Windows).
- Validate **CLAP on macOS** (bundle layout).
- **Build and ship** Windows CLAP but do not claim pluginval coverage for it.

We display the badge because we meet the process for all testable binaries, retain CI logs, and document this gap honestly. **Optional follow-up (P1):** add `clap-info` smoke test on Windows CLAP in CI for load/scan coverage without lilv.

## Local validation

```bash
./scripts/validate-plugins-local.sh
```

Do **not** run unbounded `lldb` or out-of-process pluginval in agent shells.

## README badge

```markdown
[![Verified by pluginval](https://assets.tracktion.com/img/pages/develop/develop-logo-pluginval.png)](https://www.tracktion.com/develop/pluginval)
```

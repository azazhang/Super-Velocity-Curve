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

| Platform | Formats | Tool |
|----------|---------|------|
| macOS | VST3 Instrument, VST3 MIDI FX, AU `aumf`, AU `aumi` | **pluginval** strictness 5 |
| Windows | VST3 Instrument, VST3 MIDI FX | **pluginval** strictness 5 |
| macOS | CLAP (bundle) | **[clap-validator](https://github.com/free-audio/clap-validator)** |
| Windows | CLAP (flat `.clap` DLL) | **clap-validator** |

**pluginval does not support CLAP.** Earlier CI failures on Windows CLAP were from passing a flat `.clap` DLL to pluginval (its scanner expects LV2-style bundles). The fix is **clap-validator**, which accepts the standard Windows CLAP layout directly.

CI runs pluginval **in process** (`--validate-in-process`) at strictness 5 with `--output-dir pluginval-logs`. Log artifacts: `pluginval-logs-macos`, `pluginval-logs-windows`.

## Local validation

```bash
./scripts/validate-plugins-local.sh   # VST3 + AU via pluginval
./scripts/validate-clap.sh            # CLAP via clap-validator
```

Do **not** run unbounded `lldb` or out-of-process pluginval in agent shells.

## README badge

```markdown
[![Verified by pluginval](https://assets.tracktion.com/img/pages/develop/develop-logo-pluginval.png)](https://www.tracktion.com/develop/pluginval)
```

CLAP coverage is documented here and in CI; the badge applies to VST3/AU where pluginval runs.

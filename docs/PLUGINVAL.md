# Verified by pluginval

Super VelocityCurve is validated with [pluginval](https://github.com/Tracktion/pluginval) at **strictness level 5** for every format and platform we ship.

## What we validate

| Platform | Formats |
|----------|---------|
| macOS | VST3 Instrument, VST3 MIDI FX, AU `aumf`, AU `aumi`, CLAP (when built) |
| Windows | VST3 Instrument, VST3 MIDI FX, CLAP (when built) |

CI runs pluginval **in process** (`--validate-in-process`) at strictness 5 with `--output-dir pluginval-logs` so log files are retained and exit codes are reliable on Windows per the [Adding pluginval to CI](https://github.com/Tracktion/pluginval/blob/develop/docs/Adding%20pluginval%20to%20CI.md) guide.

## Local validation

After a Release build, use the bounded helper (hard 5-minute cap per plugin, in-process):

```bash
./scripts/validate-plugins-local.sh
```

Do **not** run unbounded `lldb` or out-of-process pluginval in agent/automation shells — editor tests can hang indefinitely.

## Verified by pluginval badge

Tracktion’s [Verified by pluginval](https://www.tracktion.com/develop/pluginval) program asks developers to:

1. Run pluginval at strictness ≥ 5 on all shipped formats/platforms
2. Integrate validation into the release pipeline (this repo’s GitHub Actions workflow)
3. Display the official logo on project materials

There is no central submission form anymore; retain CI log artifacts and reference the program on this README.

[![Verified by pluginval](https://www.tracktion.com/develop/pluginval)](https://www.tracktion.com/develop/pluginval)

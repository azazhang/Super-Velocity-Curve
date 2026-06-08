# Verified by pluginval

Super VelocityCurve is validated with [pluginval](https://github.com/Tracktion/pluginval) at **strictness level 5** for every format and platform we ship.

## What we validate

| Platform | Formats |
|----------|---------|
| macOS | VST3 Instrument, VST3 MIDI FX, AU `aumf`, AU `aumi`, CLAP (when built) |
| Windows | VST3 Instrument, VST3 MIDI FX, CLAP (when built) |

CI runs pluginval **out of process** (default) with `--output-dir pluginval-logs` so log files are retained per the [Adding pluginval to CI](https://github.com/Tracktion/pluginval/blob/develop/docs/Adding%20pluginval%20to%20CI.md) guide.

## Local validation

After a Release build:

```bash
# macOS — download pluginval v1.0.4, then:
pluginval --strictness-level 5 --timeout-ms 120000 --output-dir ./pluginval-logs \
  "build/SuperVelocityCurve_artefacts/Release/VST3/Super VelocityCurve.vst3"
```

Repeat for each `.vst3`, `.component`, and `.clap` you ship.

## Verified by pluginval badge

Tracktion’s [Verified by pluginval](https://www.tracktion.com/develop/pluginval) program asks developers to:

1. Run pluginval at strictness ≥ 5 on all shipped formats/platforms
2. Integrate validation into the release pipeline (this repo’s GitHub Actions workflow)
3. Display the official logo on project materials

There is no central submission form anymore; retain CI log artifacts and reference the program on this README.

[![Verified by pluginval](https://www.tracktion.com/develop/pluginval)](https://www.tracktion.com/develop/pluginval)

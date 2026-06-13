#!/usr/bin/env bash
# Developer-only: remove old bundle names from *this machine* after rebranding.
# Not shipped to users — run manually when upgrading a dev install.
set -euo pipefail

VST3_INST="${HOME}/Library/Audio/Plug-Ins/VST3"
AU_INST="${HOME}/Library/Audio/Plug-Ins/Components"
CLAP_INST="${HOME}/Library/Audio/Plug-Ins/CLAP"

LEGACY_AU=(
  "Super VelocityCurve MIDI FX.component"
  "Super_VelocityCurve MIDI FX.component"
  "Super-Velocity-Curve MIDI FX.component"
  "Super VelocityCurve.component"
  "Super_VelocityCurve.component"
  "Super-Velocity-Curve.component"
)
LEGACY_VST3=(
  "Super VelocityCurve MIDI FX.vst3"
  "Super_VelocityCurve MIDI FX.vst3"
  "Super-Velocity-Curve MIDI FX.vst3"
  "Super VelocityCurve.vst3"
  "Super_VelocityCurve.vst3"
  "Super-Velocity-Curve.vst3"
)
LEGACY_CLAP=(
  "Super VelocityCurve MIDI FX.clap"
  "Super_VelocityCurve MIDI FX.clap"
  "Super-Velocity-Curve MIDI FX.clap"
)

removed=0
for legacy in "${LEGACY_AU[@]}"; do
  if [[ -d "$AU_INST/$legacy" ]]; then
    echo "Removing $AU_INST/$legacy"
    rm -rf "$AU_INST/$legacy"
    removed=1
  fi
done
for legacy in "${LEGACY_VST3[@]}"; do
  if [[ -d "$VST3_INST/$legacy" ]]; then
    echo "Removing $VST3_INST/$legacy"
    rm -rf "$VST3_INST/$legacy"
    removed=1
  fi
done
for legacy in "${LEGACY_CLAP[@]}"; do
  if [[ -d "$CLAP_INST/$legacy" ]]; then
    echo "Removing $CLAP_INST/$legacy"
    rm -rf "$CLAP_INST/$legacy"
    removed=1
  fi
done

if (( removed == 0 )); then
  echo "No legacy bundles found."
else
  killall -9 AudioComponentRegistrar 2>/dev/null || true
  echo "Done. Quit and reopen your DAW if it was running."
fi

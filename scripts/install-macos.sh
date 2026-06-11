#!/usr/bin/env bash
# Install unsigned Super Velocity Curve builds (macOS).
set -euo pipefail

VST3_INST="$HOME/Library/Audio/Plug-Ins/VST3"
AU_INST="$HOME/Library/Audio/Plug-Ins/Components"
APP_INST="/Applications"

usage() {
  echo "Usage: $0 <path-to-dist-folder-or-zip>"
  echo "Copies VST3, AU, and Standalone .app from a CI artifact or local build."
}

if [[ $# -lt 1 ]]; then usage; exit 1; fi

SRC="$1"
WORKDIR=$(mktemp -d)
trap 'rm -rf "$WORKDIR"' EXIT

if [[ -f "$SRC" && "$SRC" == *.zip ]]; then
  unzip -q "$SRC" -d "$WORKDIR/dist"
  SRC="$WORKDIR/dist"
fi

mkdir -p "$VST3_INST" "$AU_INST"

for item in "$SRC"/*.vst3 "$SRC"/*.VST3; do
  [[ -e "$item" ]] || continue
  echo "Installing $(basename "$item") → $VST3_INST"
  rm -rf "$VST3_INST/$(basename "$item")"
  cp -R "$item" "$VST3_INST/"
done

for item in "$SRC"/*.component; do
  [[ -e "$item" ]] || continue
  echo "Installing $(basename "$item") → $AU_INST"
  rm -rf "$AU_INST/$(basename "$item")"
  cp -R "$item" "$AU_INST/"
  xattr -cr "$AU_INST/$(basename "$item")" || true
done

for item in "$SRC"/*.app; do
  [[ -e "$item" ]] || continue
  echo "Installing $(basename "$item") → $APP_INST"
  rm -rf "$APP_INST/$(basename "$item")"
  cp -R "$item" "$APP_INST/"
  xattr -cr "$APP_INST/$(basename "$item")" || true
done

for item in "$SRC"/*.clap; do
  [[ -e "$item" ]] || continue
  CLAP_INST="$HOME/Library/Audio/Plug-Ins/CLAP"
  mkdir -p "$CLAP_INST"
  echo "Installing $(basename "$item") → $CLAP_INST"
  cp -R "$item" "$CLAP_INST/"
done

echo "Done. Rescan plugins in your DAW."

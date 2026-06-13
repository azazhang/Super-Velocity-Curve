#!/usr/bin/env bash
# Install unsigned Super Velocity Curve builds (macOS).
# Copies plugins, removes Gatekeeper quarantine flags, and ad-hoc signs bundles.
set -euo pipefail

VST3_INST="$HOME/Library/Audio/Plug-Ins/VST3"
AU_INST="$HOME/Library/Audio/Plug-Ins/Components"
CLAP_INST="$HOME/Library/Audio/Plug-Ins/CLAP"
APP_INST="/Applications"

usage() {
  cat <<'EOF'
Usage: install-macos.sh [path-to-unzipped-folder-or-zip]

With no arguments, installs plugins from the same folder as this script
(for use inside the release zip with "Install Super Velocity Curve.command").

Developer examples:
  install-macos.sh ~/Downloads/SuperVelocityCurve-macOS-unsigned.zip
  install-macos.sh ./build/SuperVelocityCurveMidiFx_artefacts/Release/AU
EOF
}

if [[ $# -lt 1 ]]; then
  SRC="$(cd "$(dirname "$0")" && pwd)"
  echo "Installing from: $SRC"
else
  SRC="$1"
fi

WORKDIR=""
cleanup_workdir() { [[ -n "$WORKDIR" && -d "$WORKDIR" ]] && rm -rf "$WORKDIR"; }
trap cleanup_workdir EXIT

if [[ -f "$SRC" && "$SRC" == *.zip ]]; then
  echo "Unzipping $(basename "$SRC")…"
  WORKDIR=$(mktemp -d)
  unzip -q "$SRC" -d "$WORKDIR/dist"
  SRC="$WORKDIR/dist"
fi

if [[ ! -d "$SRC" ]]; then
  echo "Error: '$SRC' is not a folder or zip archive." >&2
  exit 1
fi

mkdir -p "$VST3_INST" "$AU_INST" "$CLAP_INST"

# Remove legacy bundle names from earlier branding so DAWs do not list duplicates.
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
for legacy in "${LEGACY_AU[@]}"; do
  [[ -d "$AU_INST/$legacy" ]] && echo "Removing legacy $legacy" && rm -rf "$AU_INST/$legacy"
done
for legacy in "${LEGACY_VST3[@]}"; do
  [[ -d "$VST3_INST/$legacy" ]] && echo "Removing legacy $legacy" && rm -rf "$VST3_INST/$legacy"
done
for legacy in "${LEGACY_CLAP[@]}"; do
  [[ -d "$CLAP_INST/$legacy" ]] && echo "Removing legacy $legacy" && rm -rf "$CLAP_INST/$legacy"
done

prepare_bundle() {
  local bundle="$1"
  xattr -cr "$bundle" 2>/dev/null || true
  dot_clean -m "$bundle" 2>/dev/null || true
  find "$bundle" -name '._*' -delete 2>/dev/null || true
  codesign --force --sign - --timestamp=none --deep "$bundle"
}

install_glob() {
  local dest="$1"
  shift
  for pattern in "$@"; do
    for item in $pattern; do
      [[ -e "$item" ]] || continue
      [[ "$(basename "$item")" == Super\ VelocityCurve* ]] && continue
      echo "Installing $(basename "$item") → $dest"
      rm -rf "$dest/$(basename "$item")"
      COPYFILE_DISABLE=1 cp -R "$item" "$dest/"
      prepare_bundle "$dest/$(basename "$item")"
    done
  done
}

echo "Installing plugins (quarantine removal + ad-hoc sign)…"

install_glob "$VST3_INST" \
  "$SRC"/*.vst3 "$SRC"/*.VST3 \
  "$SRC"/VST3/*.vst3 "$SRC"/VST3/*.VST3 \
  "$SRC"/*/*.vst3 "$SRC"/*/*.VST3

install_glob "$AU_INST" \
  "$SRC"/*.component \
  "$SRC"/AU/*.component \
  "$SRC"/*/*.component

install_glob "$CLAP_INST" \
  "$SRC"/*.clap \
  "$SRC"/CLAP/*.clap \
  "$SRC"/*/*.clap

for item in "$SRC"/*.app "$SRC"/*/*.app; do
  [[ -e "$item" ]] || continue
  echo "Installing $(basename "$item") → $APP_INST"
  rm -rf "$APP_INST/$(basename "$item")"
  COPYFILE_DISABLE=1 cp -R "$item" "$APP_INST/"
  prepare_bundle "$APP_INST/$(basename "$item")"
done

cat <<'EOF'

Installation finished.

Now:
  1. Quit your music app completely (Logic Pro → Quit Logic Pro).
  2. Open it again and rescan plug-ins if asked.

If Mac blocked the installer earlier, use System Settings → Privacy & Security
→ Open Anyway, then run "Install Super Velocity Curve" again.
EOF

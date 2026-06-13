#!/usr/bin/env bash
# Install unsigned Super Velocity Curve builds (macOS).
# Copies plugins, removes Gatekeeper quarantine flags, and ad-hoc signs bundles.
set -euo pipefail

VST3_INST="$HOME/Library/Audio/Plug-Ins/VST3"
AU_INST="$HOME/Library/Audio/Plug-Ins/Components"
CLAP_INST="$HOME/Library/Audio/Plug-Ins/CLAP"
APP_INST="/Applications"

if [[ $# -gt 1 ]]; then
  echo "Usage: install-macos.sh [folder-from-unzipped-release]" >&2
  exit 1
fi

if [[ $# -eq 0 ]]; then
  SRC="$(cd "$(dirname "$0")" && pwd)"
  echo "Installing from: $SRC"
else
  SRC="$1"
fi

WORKDIR=""
cleanup_workdir() {
  if [[ -n "${WORKDIR:-}" && -d "$WORKDIR" ]]; then
    rm -rf "$WORKDIR"
  fi
}
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

prepare_bundle() {
  local bundle="$1"
  xattr -cr "$bundle" 2>/dev/null || true
  dot_clean -m "$bundle" 2>/dev/null || true
  find "$bundle" -name '._*' -delete 2>/dev/null || true
  codesign --force --sign - --timestamp=none --deep "$bundle"
}

refresh_au_registry() {
  # macOS caches Audio Unit registrations; Logic may ignore new user-folder AUs until this restarts.
  killall -9 AudioComponentRegistrar 2>/dev/null || true
}

install_items() {
  local dest="$1"
  shift
  local item name
  for item in "$@"; do
    [[ -e "$item" ]] || continue
    name="$(basename "$item")"
    echo "Installing $name → $dest"
    rm -rf "$dest/$name"
    COPYFILE_DISABLE=1 cp -R "$item" "$dest/"
    prepare_bundle "$dest/$name"
  done
}

echo "Installing plugins…"

shopt -s nullglob

vst3_items=(
  "$SRC"/*.vst3 "$SRC"/*.VST3
  "$SRC"/VST3/*.vst3 "$SRC"/VST3/*.VST3
  "$SRC"/*/*.vst3 "$SRC"/*/*.VST3
)
au_items=(
  "$SRC"/*.component
  "$SRC"/AU/*.component
  "$SRC"/*/*.component
)
clap_items=(
  "$SRC"/*.clap
  "$SRC"/CLAP/*.clap
  "$SRC"/*/*.clap
)
app_items=(
  "$SRC"/*.app
  "$SRC"/*/*.app
)

shopt -u nullglob

if (( ${#vst3_items[@]} > 0 )); then
  install_items "$VST3_INST" "${vst3_items[@]}"
fi
if (( ${#au_items[@]} > 0 )); then
  install_items "$AU_INST" "${au_items[@]}"
  refresh_au_registry
fi
if (( ${#clap_items[@]} > 0 )); then
  install_items "$CLAP_INST" "${clap_items[@]}"
fi

for item in "${app_items[@]}"; do
  name="$(basename "$item")"
  echo "Installing $name → $APP_INST"
  rm -rf "$APP_INST/$name"
  COPYFILE_DISABLE=1 cp -R "$item" "$APP_INST/"
  prepare_bundle "$APP_INST/$name"
done

plugin_count=$((${#vst3_items[@]} + ${#au_items[@]} + ${#clap_items[@]}))
if (( plugin_count == 0 )); then
  echo "Error: no plug-in bundles found in $SRC" >&2
  exit 1
fi

cat <<'EOF'

Installation finished.

Plug-ins are in your user Library:
  ~/Library/Audio/Plug-Ins/VST3/
  ~/Library/Audio/Plug-Ins/Components/
  ~/Library/Audio/Plug-Ins/CLAP/

Now:
  1. Quit your music app completely (Logic Pro → Quit Logic Pro).
  2. Open it again.
  3. Logic Pro: Settings → Plug-in Manager → Reset & Rescan Selection if the plug-in is missing.

If Mac blocked the installer earlier, use System Settings → Privacy & Security
→ Open Anyway, then run "Install Super Velocity Curve" again.
EOF

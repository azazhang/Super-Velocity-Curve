#!/usr/bin/env bash
# Build a macOS release zip matching CI layout (plugins + user installer).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="${BUILD_DIR:-$ROOT/build}"
CONFIG="${CMAKE_BUILD_CONFIG:-Release}"
OUT_ZIP="${1:-$ROOT/SuperVelocityCurve-macOS-unsigned.zip}"
DIST="$ROOT/dist-release"

INST_VST="$BUILD/SuperVelocityCurve_artefacts/$CONFIG/VST3/Super Velocity Curve.vst3"
INST_AU="$BUILD/SuperVelocityCurve_artefacts/$CONFIG/AU/Super Velocity Curve.component"
MIDIFX_VST="$BUILD/SuperVelocityCurveMidiFx_artefacts/$CONFIG/VST3/Super Velocity Curve MIDI FX.vst3"
MIDIFX_AU="$BUILD/SuperVelocityCurveMidiFx_artefacts/$CONFIG/AU/Super Velocity Curve MIDI FX.component"
STANDALONE="$BUILD/SuperVelocityCurve_artefacts/$CONFIG/Standalone/Super Velocity Curve.app"
CLAP_DIR="$BUILD/SuperVelocityCurveMidiFx_artefacts/$CONFIG/CLAP"

need() {
  if [[ ! -e "$1" ]]; then
    echo "Missing build artefact: $1" >&2
    echo "Run: cmake --build $BUILD --config $CONFIG" >&2
    exit 1
  fi
}

need "$INST_VST"
need "$INST_AU"
need "$MIDIFX_VST"
need "$MIDIFX_AU"
need "$STANDALONE"

rm -rf "$DIST"
mkdir -p "$DIST"

cp -R "$INST_VST" "$DIST/"
cp -R "$INST_AU" "$DIST/"
cp -R "$MIDIFX_VST" "$DIST/"
cp -R "$MIDIFX_AU" "$DIST/"
cp -R "$STANDALONE" "$DIST/"

if [[ -d "$CLAP_DIR" ]]; then
  shopt -s nullglob
  for clap in "$CLAP_DIR"/*.clap; do
    [[ -e "$clap" ]] && cp -R "$clap" "$DIST/"
  done
  shopt -u nullglob
fi

cp "$ROOT/scripts/install-macos.sh" "$DIST/"
cp "$ROOT/dist-assets/macOS/Install Super Velocity Curve.command" "$DIST/"
cp "$ROOT/dist-assets/macOS/README.txt" "$DIST/"
chmod +x "$DIST/install-macos.sh"
chmod +x "$DIST/Install Super Velocity Curve.command"

rm -f "$OUT_ZIP"
(
  cd "$DIST"
  COPYFILE_DISABLE=1 zip -r -X "$OUT_ZIP" .
)

echo "Release zip: $OUT_ZIP"
echo "Contents:"
unzip -l "$OUT_ZIP" | head -30
echo "…"
unzip -l "$OUT_ZIP" | tail -5

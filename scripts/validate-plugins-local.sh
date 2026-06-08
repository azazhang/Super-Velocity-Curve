#!/usr/bin/env bash
# Local pluginval with hard timeout — avoids lldb/pluginval hangs during debug.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="$ROOT/build"
LOG_DIR="${1:-$ROOT/pluginval-logs}"
TIMEOUT_SEC="${PLUGINVAL_TIMEOUT_SEC:-300}"
STRICTNESS="${PLUGINVAL_STRICTNESS:-5}"

mkdir -p "$LOG_DIR"

find_pv() {
  if [[ -x "$ROOT/pluginval" ]]; then echo "$ROOT/pluginval"; return; fi
  if [[ -x "$ROOT/pluginval.app/Contents/MacOS/pluginval" ]]; then
    echo "$ROOT/pluginval.app/Contents/MacOS/pluginval"; return
  fi
  command -v pluginval 2>/dev/null || true
}

PV="$(find_pv)"
if [[ -z "$PV" ]]; then
  echo "pluginval not found. Download v1.0.4 from Tracktion/pluginval releases." >&2
  exit 1
fi

validate() {
  local path="$1"
  echo "=== pluginval ($STRICTNESS): $path ==="
  if command -v timeout >/dev/null 2>&1; then
    timeout "$TIMEOUT_SEC" "$PV" --validate-in-process --strictness-level "$STRICTNESS" \
      --timeout-ms 120000 --output-dir "$LOG_DIR" "$path"
  elif command -v gtimeout >/dev/null 2>&1; then
    gtimeout "$TIMEOUT_SEC" "$PV" --validate-in-process --strictness-level "$STRICTNESS" \
      --timeout-ms 120000 --output-dir "$LOG_DIR" "$path"
  else
    echo "Install coreutils for 'timeout' (brew install coreutils) — running without hard cap." >&2
    "$PV" --validate-in-process --strictness-level "$STRICTNESS" \
      --timeout-ms 120000 --output-dir "$LOG_DIR" "$path"
  fi
}

shopt -s nullglob
paths=(
  "$BUILD/SuperVelocityCurve_artefacts/Release/VST3/Super VelocityCurve.vst3"
  "$BUILD/SuperVelocityCurveMidiFx_artefacts/Release/VST3/Super VelocityCurve MIDI FX.vst3"
  "$BUILD/SuperVelocityCurve_artefacts/Release/AU/Super VelocityCurve.component"
  "$BUILD/SuperVelocityCurveMidiFx_artefacts/Release/AU/Super VelocityCurve MIDI FX.component"
  "$BUILD/SuperVelocityCurveMidiFx_artefacts/Release/CLAP/"*.clap
)

found=0
for p in "${paths[@]}"; do
  [[ -e "$p" ]] || continue
  found=1
  validate "$p"
done

if (( found == 0 )); then
  echo "No built plugins found under $BUILD. Run cmake --build first." >&2
  exit 1
fi

echo "All pluginval checks passed."

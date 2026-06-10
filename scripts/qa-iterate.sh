#!/usr/bin/env bash
# Iterative QA gate: build → engine tests → optional pluginval → smoke checklist.
# Usage: ./scripts/qa-iterate.sh [--pluginval] [--round N]
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="${BUILD_DIR:-$ROOT/build}"
CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:-RelWithDebInfo}"
RUN_PLUGINVAL=0
ROUND=1

for arg in "$@"; do
  case "$arg" in
    --pluginval) RUN_PLUGINVAL=1 ;;
    --round=*) ROUND="${arg#*=}" ;;
    --round) shift; ROUND="${1:-1}" ;;
  esac
done

log() { printf '\n[qa-iterate round %s] %s\n' "$ROUND" "$*"; }
fail() { log "FAIL: $*"; exit 1; }

log "Configure + build ($CMAKE_BUILD_TYPE)"
cmake -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" -DSVC_ENABLE_LTO=OFF >/dev/null
cmake --build "$BUILD" --parallel

log "Tests (ctest: engine + layout)"
ctest --test-dir "$BUILD" --output-on-failure

STANDALONE="$BUILD/SuperVelocityCurve_artefacts/$CMAKE_BUILD_TYPE/Standalone/Super VelocityCurve.app"
BINARY="$STANDALONE/Contents/MacOS/Super VelocityCurve"
[[ -x "$BINARY" ]] || fail "Standalone binary missing: $BINARY"

if [[ "$(uname)" == "Darwin" ]]; then
  TS=$(stat -f "%Sm" -t "%Y-%m-%d %H:%M:%S" "$BINARY")
else
  TS=$(stat -c "%y" "$BINARY" 2>/dev/null | cut -d. -f1)
fi
log "Standalone ready: $STANDALONE ($TS)"

if (( RUN_PLUGINVAL )); then
  log "pluginval (strictness 5)"
  CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" PLUGINVAL_TIMEOUT_SEC=300 "$ROOT/scripts/validate-plugins-local.sh"
  log "clap-validator"
  CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" "$ROOT/scripts/validate-clap.sh"
fi

log "Agentic QA packs (docs/developer/AGENTIC_QA.md) — run in Standalone:"
cat <<'EOF'
  Pack A (core): Launchpad scrollbar w/o resize | live hits linger 3s | min-window curve | pad-switch persistence
  Pack B (profile): template name typing | MIDI channel change | dirty dialog only on profile switch
  Pack C (pads): Launchpad cell text | MIDI note label | AT curve toggle back
  Pack D (perf): idle CPU ok | profile switch responsive
  Pack E (about): developer text | ko-fi link
  Pack F (cal/hist): calibration 3-step | histogram 60-90 band
  Full matrix: docs/developer/TESTING.md
EOF

log "Round $ROUND complete."

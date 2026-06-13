#!/usr/bin/env bash
# End-to-end check: unzip layout + install-macos.sh with no args (same as double-click .command).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

usage() {
  cat <<'EOF'
Usage: verify-macos-installer.sh <dist-folder-or-release-zip>

Simulates a normal user install:
  1. Use the unzipped release folder (or unzip a .zip into a temp dir)
  2. cd into that folder
  3. run install-macos.sh with no arguments

Plugin destinations are checked under a temporary HOME.
EOF
}

if [[ $# -ne 1 ]]; then
  usage >&2
  exit 1
fi

INPUT="$1"
WORKDIR=""
FAKE_HOME=""
cleanup() {
  [[ -n "$WORKDIR" && -d "$WORKDIR" ]] && rm -rf "$WORKDIR"
  [[ -n "$FAKE_HOME" && -d "$FAKE_HOME" ]] && rm -rf "$FAKE_HOME"
}
trap cleanup EXIT

DIST=""
if [[ -f "$INPUT" && "$INPUT" == *.zip ]]; then
  WORKDIR=$(mktemp -d)
  DIST="$WORKDIR/dist"
  unzip -q "$INPUT" -d "$DIST"
elif [[ -d "$INPUT" ]]; then
  DIST="$(cd "$INPUT" && pwd)"
else
  echo "Error: '$INPUT' is not a folder or .zip archive." >&2
  exit 1
fi

for required in install-macos.sh "Install Super Velocity Curve.command"; do
  if [[ ! -e "$DIST/$required" ]]; then
    echo "Error: release folder missing '$required'." >&2
    exit 1
  fi
done

FAKE_HOME=$(mktemp -d)
export HOME="$FAKE_HOME"

echo "=== verify-macos-installer: dist=$DIST HOME=$FAKE_HOME ==="
set +e
(
  cd "$DIST"
  bash ./install-macos.sh
)
install_exit=$?
set -e
if (( install_exit != 0 )); then
  echo "install-macos.sh exited $install_exit" >&2
fi

VST3="$HOME/Library/Audio/Plug-Ins/VST3"
AU="$HOME/Library/Audio/Plug-Ins/Components"
CLAP="$HOME/Library/Audio/Plug-Ins/CLAP"

REQUIRED_VST3=(
  "Super Velocity Curve.vst3"
  "Super Velocity Curve MIDI FX.vst3"
)
REQUIRED_AU=(
  "Super Velocity Curve.component"
  "Super Velocity Curve MIDI FX.component"
)
REQUIRED_CLAP=(
  "Super Velocity Curve MIDI FX.clap"
)

missing=0
check_dir() {
  local dir="$1"
  shift
  local name
  for name in "$@"; do
    if [[ ! -d "$dir/$name" ]]; then
      echo "MISSING: $dir/$name" >&2
      missing=1
    else
      echo "OK: $dir/$name"
    fi
  done
}

check_dir "$VST3" "${REQUIRED_VST3[@]}"
check_dir "$AU" "${REQUIRED_AU[@]}"
check_dir "$CLAP" "${REQUIRED_CLAP[@]}"

if (( missing != 0 )); then
  echo "verify-macos-installer: FAILED — plug-ins not installed (user path broken)." >&2
  exit 1
fi

if (( install_exit != 0 )); then
  echo "verify-macos-installer: FAILED — install-macos.sh exited $install_exit." >&2
  exit 1
fi

echo "verify-macos-installer: PASSED"

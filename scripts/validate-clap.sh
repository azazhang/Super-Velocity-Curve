#!/usr/bin/env bash
# Validate CLAP with clap-validator (pluginval does not support CLAP).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="${BUILD_DIR:-$ROOT/build}"
CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:-RelWithDebInfo}"
CLAP_VALIDATOR_VERSION="${CLAP_VALIDATOR_VERSION:-0.3.2}"

find_clap() {
  local paths=(
    "$BUILD/SuperVelocityCurveMidiFx_artefacts/$CMAKE_BUILD_TYPE/CLAP/"*.clap
    "$BUILD/SuperVelocityCurveMidiFx_artefacts/Release/CLAP/"*.clap
  )
  shopt -s nullglob
  for p in "${paths[@]}"; do
    [[ -e "$p" ]] || continue
    echo "$p"
    return 0
  done
  return 1
}

install_clap_validator() {
  local dir="$ROOT/.cache/clap-validator"
  mkdir -p "$dir"
  if [[ "$(uname)" == "Darwin" ]]; then
    local archive="$dir/clap-validator-$CLAP_VALIDATOR_VERSION-macos-universal.tar.gz"
    if [[ ! -x "$dir/binaries/clap-validator" ]]; then
      curl -fsSL -o "$archive" \
        "https://github.com/free-audio/clap-validator/releases/download/$CLAP_VALIDATOR_VERSION/clap-validator-$CLAP_VALIDATOR_VERSION-macos-universal.tar.gz"
      tar -xzf "$archive" -C "$dir"
      chmod +x "$dir/binaries/clap-validator"
    fi
    echo "$dir/binaries/clap-validator"
  else
    echo "Install clap-validator from https://github.com/free-audio/clap-validator/releases" >&2
    return 1
  fi
}

CLAP_PATH="$(find_clap)" || {
  echo "No CLAP bundle found under $BUILD. Build with -DSVC_BUILD_CLAP=ON first." >&2
  exit 1
}

CV="$(command -v clap-validator 2>/dev/null || true)"
if [[ -z "$CV" ]]; then
  CV="$(install_clap_validator)"
fi

if [[ "$(uname)" == "Darwin" && -d "$CLAP_PATH" ]]; then
  while IFS= read -r -d '' binary; do
    codesign --force --sign - --timestamp=none "$binary" 2>/dev/null || true
  done < <(find "$CLAP_PATH/Contents/MacOS" -type f -perm +111 -print0 2>/dev/null)
  codesign --force --sign - --timestamp=none --deep "$CLAP_PATH" 2>/dev/null || true
  xattr -cr "$CLAP_PATH" 2>/dev/null || true
fi

echo "=== clap-validator: $CLAP_PATH ==="
"$CV" validate "$CLAP_PATH"
echo "CLAP validation passed."

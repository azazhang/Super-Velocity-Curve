#!/usr/bin/env bash
# Fail on deprecated name spellings. Allowed: "Super Velocity Curve" | SuperVelocityCurve
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

FORBIDDEN=(
  'Super VelocityCurve'
  'Super-Velocity-Curve'
  'Super_VelocityCurve'
)
found=0

while IFS= read -r -d '' f; do
  for bad in "${FORBIDDEN[@]}"; do
    if grep -q "$bad" "$f"; then
      echo "FORBIDDEN '$bad' in $f"
      grep -n "$bad" "$f" || true
      found=1
    fi
  done
done < <(git ls-files -z \
  ':!JUCE' ':!third_party' \
  ':!scripts/check-branding.sh' \
  ':!scripts/install-macos.sh' \
  ':!scripts/dev/cleanup-legacy-plugin-bundles.sh' \
  '*.md' '*.h' '*.cpp' '*.sh' '*.yml' 'CMakeLists.txt')

if (( found != 0 )); then
  echo "Branding check failed. Use Super Velocity Curve or SuperVelocityCurve only." >&2
  exit 1
fi

echo "Branding check passed."

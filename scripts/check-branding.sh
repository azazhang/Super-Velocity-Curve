#!/usr/bin/env bash
# Fail if the deprecated hybrid brand "Super VelocityCurve" appears in tracked project files.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

HYBRID='Super VelocityCurve'
found=0

while IFS= read -r -d '' f; do
  if grep -q "$HYBRID" "$f"; then
    echo "HYBRID BRAND (use 'Super Velocity Curve'): $f"
    grep -n "$HYBRID" "$f" || true
    found=1
  fi
done < <(git ls-files -z \
  ':!JUCE' ':!third_party' \
  ':!docs/developer/NAMING.md' ':!scripts/check-branding.sh' \
  '*.md' '*.h' '*.cpp' '*.sh' '*.yml' 'CMakeLists.txt')

if (( found != 0 )); then
  echo "Branding check failed. See docs/developer/NAMING.md" >&2
  exit 1
fi

echo "Branding check passed (no hybrid '$HYBRID')."

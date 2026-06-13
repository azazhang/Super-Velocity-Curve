#!/bin/bash
# Double-click installer for Super Velocity Curve (macOS).
# Copies plugins into your user Library and prepares them for Logic / Ableton / etc.
cd "$(dirname "$0")"
bash "$(dirname "$0")/install-macos.sh"
echo ""
read -r -p "Press Return to close this window…" _

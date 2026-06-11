# Publishing a GitHub Release

Releases are **not automatic on every merge**. They run when:

1. A maintainer pushes a git tag `v*.*.*` (e.g. `v0.2.0`), **or**
2. **Actions → Release → Run workflow** (manual `workflow_dispatch` with version input).

## Before tagging

- [ ] `./scripts/qa-iterate.sh --pluginval` green (or CI main/nightly pluginval green)
- [ ] Profile switch dialog works in Standalone + at least one DAW
- [ ] [DAW_TEST_MATRIX.md](DAW_TEST_MATRIX.md) smoke rows ticked
- [ ] `CHANGELOG.md` updated
- [ ] Version in `CMakeLists.txt` matches tag

## Create release

```bash
git tag v0.2.0
git push origin v0.2.0
```

Or use **workflow_dispatch** on the Release workflow (no tag required for pre-releases).

Artifacts: `SuperVelocityCurve-macOS-unsigned.zip`, `SuperVelocityCurve-Windows-unsigned.zip`.

Each zip includes VST3 + Standalone; macOS adds AU bundles; **both platforms** include `Super Velocity Curve MIDI FX.clap` (flat `.clap` on Windows).

# Changelog

## v0.6.2

### Added
- **Add pad** / **Delete pad** on Pad Layout — build kits from Blank Custom or trim factory templates
- Save-in-place for `[My]` profiles; Save on factory template creates a new user copy
- About panel (version, pluginval badge, links)
- Per-group zone routing channel combos; scrollable Routing tab
- Dynamic pad grid columns (8 for Launchpad, 4 for other profiles)
- Engine tests for pad add/remove and duplicate MIDI key rejection
- QA docs: `docs/developer/TESTING.md`, pluginval exception notes

### Fixed
- A/B audition stays in sync after profile/engine updates (BUG-001)
- Pad inspector no longer clears live hit flashes (BUG-002)
- Copy curve no longer breaks pad selection
- Live hits row vs tabs layout overlap; tab panel height
- Launchpad 8×8 grid overlap

## v0.6.0

### Added
- Logic-inspired dark theme + optional light theme (Appearance menu)
- Input gate handles: horizontal input range + vertical output at gates
- User docs: [Install](docs/user/install.md), [Getting started](docs/user/getting-started.md)
- GitHub Releases workflow (tag `v*.*.*`)
- `AGENTS.md` for AI agents; developer docs under `docs/developer/`

### Fixed
- Toolbar layout overlap (Routing / Remap / MIDI meters) — tabbed panel
- Endpoint handles restore vertical drag while keeping input gating
- Standalone `.app` bundle timestamp refreshed on rebuild (CMake)

## v0.5.3
- Windows pluginval: quote VST3 paths containing spaces

## v0.5.0
- Dual VST3/AU builds, CLAP, MIDI 2.0 LUT, factory `.svcp` exports

## v0.4.0
- Full audit backlog: remap UI, histograms, calibration, standalone MIDI I/O

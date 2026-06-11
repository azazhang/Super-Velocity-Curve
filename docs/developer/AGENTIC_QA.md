# Agentic QA workflow

Super Velocity Curve bugs often live at the intersection of **JUCE widget side effects**, **timer-driven UI**, and **multi-step user flows** — areas that pure `ctest` assertions miss. This document defines how humans and coding agents should validate changes **before** marking work done.

## Principles

1. **Scripted tests catch math; agents catch feel and flow.**
2. **Never close a UI bug from code review alone** — run the built Standalone and execute the relevant scenario pack below.
3. **Every user-reported UI bug becomes a regression scenario** (scripted if cheap, agentic if not).
4. **Record evidence** — screenshot or one-line observation per scenario (pass/fail).

## Layered workflow

```
┌─────────────────────────────────────────────────────────────┐
│  Agent scenario packs (this doc) — interactive, visual      │
├─────────────────────────────────────────────────────────────┤
│  qa-iterate.sh — build + ctest + checklist reminder         │
├─────────────────────────────────────────────────────────────┤
│  EngineTests + LayoutTests — deterministic core               │
├─────────────────────────────────────────────────────────────┤
│  pluginval / clap-validator — host API stability            │
└─────────────────────────────────────────────────────────────┘
```

## Agent pre-ship ritual

After `cmake --build` and `ctest`:

1. Open **Standalone** from the build artefacts path printed by `qa-iterate.sh`.
2. Run **Pack A** (always) and any packs matching files you touched (see mapping table).
3. If anything fails, fix and repeat — do not update `BACKLOG.md` to resolved.

### Pack A — Core editor (every UI touch)

| Step | Action | Pass criteria |
|------|--------|---------------|
| A1 | Load `[Template] Launchpad Drum Rack` | Pad grid shows vertical scrollbar without resizing window |
| A2 | Select pad, play MIDI (or standalone input) | Live hits line shows `in→out` for **≥3 seconds** after last hit |
| A3 | Resize to minimum window height | Velocity curve plot clearly visible |
| A4 | Switch pad after editing curve | Edits preserved on return |

### Pack B — Profile & dirty state (`PluginEditor`, `ProfileStore`)

| Step | Action | Pass criteria |
|------|--------|---------------|
| B1 | `[Template] Blank Custom` — type in profile name field | No save dialog while typing |
| B2 | Change pad MIDI channel | Channel sticks; no spurious save dialog |
| B3 | Edit pad on template, switch profile | Save / Discard / Cancel appears once (not on every keystroke) |

### Pack C — Pad inspector & labels (`PadInspector`, `PadGrid`)

| Step | Action | Pass criteria |
|------|--------|---------------|
| C1 | Launchpad template pad cells | Note name + `Ch N` readable, not truncated |
| C2 | Inspector MIDI note slider | Shows `36 (C1)` style label |
| C3 | Edit AT curve → same button | Returns to velocity curve; title says `Velocity curve` |

### Pack D — Performance (`timerCallback`, meters, histograms)

| Step | Action | Pass criteria |
|------|--------|---------------|
| D1 | Open editor, idle 10 s | CPU/GPU not pegged (Activity Monitor spot check) |
| D2 | Switch Launchpad → Blank → Launchpad | UI remains responsive; no multi-second freeze |

### Pack E — About & links (`AboutPanel`, `AppUrls`)

| Step | Action | Pass criteria |
|------|--------|---------------|
| E1 | Open About | Developer line readable (no `Â·` mojibake) |
| E2 | Support links (from README) | BMC `buymeacoffee.com/azhang` and Ko-fi `ko-fi.com/studioj` |

### Pack F — Calibration & histograms

| Step | Action | Pass criteria |
|------|--------|---------------|
| F1 | Expand Calibration wizard | Step 1/3 shown (not stale “complete”) |
| F2 | Soft / medium / hard hits on selected pad | Preview curve is non-flat; Apply enables |
| F3 | Expand Histograms, play pads | Shaded **60–90** band visible; legend explains touch range |

## File → pack mapping

| Touched paths | Run packs |
|---------------|-----------|
| `Source/Plugin/PluginEditor.*` | A, B, D |
| `Source/UI/PadGrid*` | A, C |
| `Source/UI/PadInspector*` | B, C |
| `Source/UI/CurveEditor*` | A, C |
| `Source/UI/CalibrationWizard*` | F |
| `Source/UI/Histogram*` | F |
| `Source/UI/AboutPanel*` | E |
| `Source/Engine/*` only | `ctest` + Pack A2 (live hits) |

## Promoting agent findings to automated tests

| Finding type | Promotion path |
|--------------|----------------|
| Layout geometry, scrollbar visibility | `Tests/LayoutTests.cpp` |
| Velocity / MIDI routing math | `Tests/EngineTests.cpp` |
| Widget side effect (e.g. `ComboBox::clear` fires onChange) | Engine/layout test if simulable; else **permanent row in Pack B** |
| Visual truncation / encoding | Layout test or Pack C screenshot gate |
| Perf / idle repaint | Pack D + comment in `TESTING.md` postmortem |

## Prompt template for coding agents

When fixing a UI bug, end your session with:

```
Built: <Standalone.app path + timestamp>
ctest: pass
Pack A: pass/fail (+ note)
Pack <X>: pass/fail (+ note)
New regression: <test added | scenario row added to AGENTIC_QA.md>
```

## Related

- [TESTING.md](TESTING.md) — pyramid, manual matrix, postmortem
- `./scripts/qa-iterate.sh` — build + test entry point

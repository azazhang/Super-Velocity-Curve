# Getting started

**Super Velocity Curve** maps **how hard you hit** to **MIDI velocity** — per pad, per profile. Start here whether you finger-drum on a grid, play an e-kit into VST drums, or just need more than one global curve in your DAW.

## Quick workflow

1. **Load the plugin** in your DAW ([Install](install.md)).
2. Pick a **factory profile** matching your controller (GM, Launchpad, Maschine, etc.).
3. Click a **pad** in the Pad Layout grid.
4. In **Pad Settings**, set **name**, **MIDI note**, and **channel** to match your hardware.
5. Drag the **velocity curve** — gate handles on left/right set input range.
6. Play pads — **live hits** show on the curve; **histograms** show your dynamics.

## Which plugin should I use?

Same table as the [README](../README.md#which-plugin-do-i-need). Quick reference:

| DAW | Plugin | Formats | Slot |
|-----|--------|---------|------|
| Ableton Live | Super Velocity Curve | VST3 Instrument | Instrument on MIDI track (see below) |
| Logic Pro | Super Velocity Curve MIDI FX | AU | MIDI FX above instrument |
| Reaper | Super Velocity Curve MIDI FX | VST3, CLAP; AU on macOS | Track input FX |
| Bitwig | Super Velocity Curve MIDI FX | CLAP (preferred), VST3 | Note FX |
| FL Studio | Super Velocity Curve MIDI FX | VST3 | MIDI effect on channel rack |
| Cubase / Nuendo | Super Velocity Curve MIDI FX | VST3 | MIDI Modifiers |
| Studio One | Super Velocity Curve MIDI FX | VST3 | Event FX / MIDI FX |
| Standalone | Super Velocity Curve | app / exe | IAC / loopMIDI to DAW |

## Ableton Live

Ableton blocks third-party plugins in the **MIDI FX** slot.

1. Create a MIDI track with **`Super Velocity Curve.vst3`** (Instrument).
2. On your **drum track**, set **MIDI From** → that track’s output.
3. Mute the instrument track’s audio if it makes sound — only MIDI routing matters.

## Logic Pro

1. On a software instrument track, open the **MIDI FX** slot (above the instrument).
2. Load **`Super Velocity Curve MIDI FX.component`** (AU).
3. Your drum plugin (Kontakt, Drum Kit Designer, etc.) stays in the instrument slot.

## Reaper

1. Add **`Super Velocity Curve MIDI FX`** as a **track input FX** (or container FX before the instrument).
2. **macOS:** AU `.component` or VST3 or CLAP (`Super Velocity Curve MIDI FX.clap`, Reaper 7+).
3. **Windows:** VST3 or CLAP.
4. Ensure MIDI reaches the track before the drum plugin.

## Bitwig

1. On a note lane, add **`Super Velocity Curve MIDI FX`** as a **Note FX**.
2. CLAP is the preferred format on Bitwig; VST3 also works.

## FL Studio

1. Add **`Super Velocity Curve MIDI FX.vst3`** to the channel as a **MIDI effect** (not a generator).
2. Place it **before** your FPC / sampler in the chain so velocities are shaped pre-instrument.

## Cubase / Nuendo

1. Open the track’s **MIDI Modifiers** or insert as MIDI plugin (depends on version).
2. Use **`Super Velocity Curve MIDI FX.vst3`**.

## Pad setup

Each pad maps incoming MIDI (note + channel) to its own curve.

1. Select a pad in **Pad Layout**.
2. In **Pad Settings**, edit name, MIDI note (0–127), channel.
3. **Add pad** / **Delete pad** to build a custom kit.
4. Play the pad — grid flash + curve hit marker should respond.

### Save your work

- **Save Profile** — stores under `[My] …` (factory templates are read-only until saved).
- Switching profiles with unsaved edits prompts **Save / Discard / Cancel**.
- **Export** / **Import** `.svcp` files for backup and sharing.

## A/B compare

1. **Capture A** → edit curve → **Hear A** toggles audition.
2. Gold overlay = alternate curve.

## Collapsible panels

- **> Histograms** — per-pad and global velocity
- **> MIDI routing & remap** — channel filter, humanize, note remap
- **> Calibration wizard** — curve from live playing
- **v Pad settings** — right column; drag section bottom edge to resize

## Curve editor tips

- **Gold / violet gate handles** — input range and output at gates.
- **Double-click** add point; **right-click** remove.
- Live hits show a crosshair on the curve with **in→out** velocity label.

## Virtual MIDI (Standalone)

- **macOS:** Audio MIDI Setup → **IAC Driver**.
- **Windows:** [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html).

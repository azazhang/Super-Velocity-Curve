# Getting started

Super VelocityCurve shapes **how hard you hit** maps to **MIDI velocity** — per pad, per profile.

## Quick workflow

1. **Load the plugin** in your DAW (see [Install](install.md)).
2. Pick a **factory profile** matching your controller (GM, Launchpad, Maschine, etc.).
3. Click a **pad** in the Pad Layout grid.
4. In **Pad Settings** (below the curve), set **pad name**, **MIDI note**, and **channel** to match your hardware.
5. Drag the **velocity curve** — use gate handles on the left/right for input range.
6. Play your pads — watch **live hits** and the **histogram**.

## Which plugin should I use?

| DAW | Install this | Where to put it |
|-----|--------------|-----------------|
| **Ableton Live** | `Super VelocityCurve` (Instrument) | Instruments on a MIDI track |
| **Logic Pro** | `Super VelocityCurve MIDI FX` | MIDI FX slot on instrument track |
| **Reaper / Bitwig** | `Super VelocityCurve MIDI FX` | MIDI FX / Note FX chain |
| **No DAW** | Standalone `.app` / `.exe` | Run directly; route MIDI via IAC / loopMIDI |

## Ableton Live

Ableton does **not** allow third-party plugins in the MIDI FX slot.

1. Load **`Super VelocityCurve.vst3`** (Instrument) on a MIDI track.
2. On your **drum track**, set **MIDI From** → that track’s output.
3. Mute or ignore the instrument track’s audio — only MIDI is used.

## Logic Pro

1. Load **`Super VelocityCurve MIDI FX.component`** in the **MIDI FX** slot (above the instrument).
2. Your drum plugin sits below as usual.

## Pad setup

Each pad maps incoming MIDI (note + channel) to its own curve.

1. Select a pad in **Pad Layout**.
2. Under **Pad Settings**, edit **Pad name**, **MIDI note (0–127)**, and **MIDI channel**.
3. Use **Add pad** / **Delete pad** above the grid to build a custom kit (you cannot delete the last pad).
4. Play that pad on your controller — the pad should flash and the histogram should update.

### Create a profile from scratch

1. Profile dropdown → **[Template] Blank Custom** (starts with one pad).
2. **Add pad** for each drum/cymbal; set note + channel to match your controller.
3. Shape curves per pad.
4. Type a name in **My Profile** → **Save Profile** (creates `[My] …` entry).

### Customize a factory template (e.g. GM)

1. Load **[Template] GM Standard** (or Launchpad, Maschine, etc.).
2. Select pads you do not use → **Delete pad**.
3. **Add pad** for extra notes you need.
4. **Save Profile** to store as your own `[My] …` profile (factory templates are not overwritten).

Use the **Note Remap** tab if your controller sends different notes than the profile expects (remap runs before velocity shaping).

## A/B compare (Capture A / Hear A)

Compare a curve before and after edits:

1. Shape a curve, then click **Capture A** (button highlights).
2. Edit the curve — your changes are the “B” version.
3. Click **Hear A** to toggle audition between captured **A** and your **edits**.
4. The **gold line** on the graph is the alternate curve.
5. Play the pad while toggling — outgoing MIDI velocity follows the auditioned curve.
6. Select another pad to exit compare mode (your edits are kept).

## Curve editor tips

- **Left gate (gold):** drag **right** to ignore soft ghost hits; drag **up** to set output at the gate.
- **Right gate (violet):** drag **left** to cap input range; drag **up/down** for output at the ceiling.
- **Double-click** the curve to add a point; **right-click** a point to remove it.
- Use **Calibration** (bottom) to match your playing dynamics.

## Virtual MIDI (Standalone)

- **macOS:** Audio MIDI Setup → enable **IAC Driver** → create a bus.
- **Windows:** install [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html).

Route Standalone **output** to your DAW **input**.

## Save your work

- **Save Profile** stores your pad curves under a custom name.
- **Export** saves a `.svcp` file you can share or back up.
- **Import** loads someone else’s profile.

# Getting started

Super VelocityCurve shapes **how hard you hit** maps to **MIDI velocity** — per pad, per profile.

## Quick workflow

1. **Load the plugin** in your DAW (see [Install](install.md)).
2. Pick a **factory profile** matching your controller (GM, Launchpad, Maschine, etc.).
3. Click a **pad** in the grid.
4. Drag the **velocity curve** — use gate handles on the left/right for input range.
5. Play your pads — watch **live hits** and the **histogram**.

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

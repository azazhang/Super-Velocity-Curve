# Install Super Velocity Curve

## Mac

### Step 1 — Download

1. Open [Releases](https://github.com/azazhang/SuperVelocityCurve/releases/latest).
2. Download **`SuperVelocityCurve-macOS-unsigned.zip`**.

### Step 2 — Unzip

Double-click the zip file. Open the folder that appears.

### Step 3 — Install

1. Double-click **`Install Super Velocity Curve`**.
2. Wait until the window says installation finished.
3. Press **Return** to close the window.

If Mac blocks the installer the first time:

1. Open **System Settings** → **Privacy & Security**.
2. Click **Open Anyway** next to the blocked installer message.
3. Double-click **`Install Super Velocity Curve`** again.

### Step 4 — Restart your DAW

1. Quit your music app completely (for example **Logic Pro → Quit Logic Pro**).
2. Open it again.
3. Rescan plug-ins if prompted (Logic: **Settings → Plug-in Manager → Reset & Rescan Selection**).

### Step 5 — Add the plugin

| DAW | Plugin name | Where to add it |
|-----|-------------|-----------------|
| **Logic Pro** | Super Velocity Curve MIDI FX | Track → **MIDI FX** slot |
| **Ableton Live** | Super Velocity Curve | **Instrument** on a MIDI track |
| **Bitwig, Reaper 7+, etc.** | Super Velocity Curve MIDI FX | CLAP MIDI FX slot (if your host supports CLAP) |
| **Without a DAW** | Super Velocity Curve | **Applications** (installed by the installer) |

See [Getting started](getting-started.md) for screenshots and first-use tips.

---

## Windows

1. Download **`SuperVelocityCurve-Windows-unsigned.zip`** from [Releases](https://github.com/azazhang/SuperVelocityCurve/releases/latest).
2. Unzip the file.
3. Copy the folder **`Super Velocity Curve.vst3`** to `C:\Program Files\Common Files\VST3\`.
4. Copy **`Super Velocity Curve MIDI FX.vst3`** to the same folder if you use it.
5. Copy **`Super Velocity Curve MIDI FX.clap`** to `C:\Program Files\Common Files\CLAP\` if your host uses CLAP.
6. If Windows shows a blocked warning: right-click the folder → **Properties** → check **Unblock** → OK.
7. Quit your DAW, reopen it, and rescan plug-ins.

---

## Manual install (Mac)

| Folder in the zip | Destination |
|-------------------|-------------|
| `Super Velocity Curve.vst3` | `~/Library/Audio/Plug-Ins/VST3/` |
| `Super Velocity Curve MIDI FX.vst3` | `~/Library/Audio/Plug-Ins/VST3/` |
| `Super Velocity Curve.component` | `~/Library/Audio/Plug-Ins/Components/` |
| `Super Velocity Curve MIDI FX.component` | `~/Library/Audio/Plug-Ins/Components/` |
| `Super Velocity Curve MIDI FX.clap` | `~/Library/Audio/Plug-Ins/CLAP/` |
| `Super Velocity Curve.app` | Applications |

In Finder, press **Cmd+Shift+G** and paste `~/Library/Audio/Plug-Ins/VST3/` to open the VST3 folder.

After copying, run **`Install Super Velocity Curve`** from the same unzipped folder so plug-ins are prepared for your DAW.

---

## Troubleshooting

**Plugin missing or wrong version**  
Download a fresh zip, unzip, run **Install Super Velocity Curve**, quit the DAW fully, and rescan.

**Logic Pro**  
Add **Super Velocity Curve MIDI FX** in the track **MIDI FX** slot.

**Ableton Live**  
Add **Super Velocity Curve** as an **Instrument** on a MIDI track.

**Windows**  
Copy the whole `.vst3` folder, not files from inside it.

**Still stuck?**  
[Open an issue](https://github.com/azazhang/SuperVelocityCurve/issues) with your OS, DAW, and what happened.

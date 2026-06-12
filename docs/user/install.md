# Install Super Velocity Curve

No coding required. Pick the download for your computer, unzip, and copy the plugins into the folders below.

## Step 1 — Download

1. Open **[Releases](https://github.com/azazhang/SuperVelocityCurve/releases/latest)** on GitHub.
2. Download **`SuperVelocityCurve-macOS-unsigned.zip`** (Mac) or **`SuperVelocityCurve-Windows-unsigned.zip`** (Windows).

> Builds are **unsigned** (free/open source). macOS may ask you to allow the plugin once — see [Troubleshooting](#troubleshooting) below.

## Step 2 — Unzip

Double-click the zip file. You should see folders like:

- `Super Velocity Curve.vst3`
- `Super Velocity Curve MIDI FX.vst3` (and/or `.component` on Mac)
- `Super Velocity Curve MIDI FX.clap` (Mac and Windows — MIDI FX only)
- `Super Velocity Curve.app` (Mac standalone) or `Super Velocity Curve.exe` (Windows standalone)

> **Upgrading from v0.2.8 or older?** Bundles were renamed to **Super Velocity Curve** (three words). Delete pre-v0.2.9 copies from your plug-in folders before copying the new ones, then rescan — otherwise your DAW may list both.

## Step 3 — Copy plugins to the right place

### macOS

| What you downloaded | Copy to |
|---------------------|---------|
| `Super Velocity Curve.vst3` | `~/Library/Audio/Plug-Ins/VST3/` |
| `Super Velocity Curve MIDI FX.vst3` | `~/Library/Audio/Plug-Ins/VST3/` |
| `Super Velocity Curve.component` | `~/Library/Audio/Plug-Ins/Components/` |
| `Super Velocity Curve MIDI FX.component` | `~/Library/Audio/Plug-Ins/Components/` |
| `Super Velocity Curve MIDI FX.clap` | `~/Library/Audio/Plug-Ins/CLAP/` (Bitwig, Reaper 7+, etc.) |
| `Super Velocity Curve.app` | Applications folder (optional — runs without a DAW) |

> **CLAP = MIDI FX only (intentional).** CLAP hosts load it in the note/MIDI FX chain. Ableton’s instrument slot needs VST3 — see [Which plugin do I need?](../../README.md#which-plugin-do-i-need).

**Finder shortcut:** In Finder, press **Cmd+Shift+G** and paste `~/Library/Audio/Plug-Ins/VST3/`.

### Windows

| What you downloaded | Copy to |
|---------------------|---------|
| `Super Velocity Curve.vst3` | `C:\Program Files\Common Files\VST3\` |
| `Super Velocity Curve MIDI FX.vst3` | `C:\Program Files\Common Files\VST3\` |
| `Super Velocity Curve MIDI FX.clap` | `C:\Program Files\Common Files\CLAP\` (Bitwig, Reaper 7+, etc.) |
| `Super Velocity Curve.exe` | Any folder you like (standalone) |

## Step 4 — Rescan in your DAW

Open your DAW’s plugin preferences and **rescan VST3** (and AU on Mac, **CLAP** if your host uses it).

- **Ableton Live:** use the **Instrument** build only — see [Getting started](getting-started.md#ableton-live).
- **Logic Pro:** use **`Super Velocity Curve MIDI FX.component` (AU only)** in the MIDI FX slot — Logic does not load VST3. See [Getting started](getting-started.md#logic-pro).

## Troubleshooting

### macOS: “plugin cannot be opened” / security warning

In Terminal, run (adjust the path if needed):

```bash
xattr -cr ~/Library/Audio/Plug-Ins/VST3/Super\ VelocityCurve.vst3
xattr -cr ~/Library/Audio/Plug-Ins/VST3/Super\ VelocityCurve\ MIDI\ FX.vst3
```

Or: **System Settings → Privacy & Security → Open Anyway** after the first blocked load.

### Windows: plugin doesn’t appear

Confirm the `.vst3` folder (not just files inside it) is in `Common Files\VST3\`, then rescan in your DAW. For CLAP hosts, copy `Super Velocity Curve MIDI FX.clap` to `C:\Program Files\Common Files\CLAP\` and rescan CLAP.

### Still stuck?

Open a [GitHub Issue](https://github.com/azazhang/SuperVelocityCurve/issues) and say your OS, DAW, and which plugin variant you installed.

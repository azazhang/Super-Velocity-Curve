# Install Super VelocityCurve

No coding required. Pick the download for your computer, unzip, and copy the plugins into the folders below.

## Step 1 — Download

1. Open the **[Releases](https://github.com/azazhang/Super_VelocityCurve/releases)** page on GitHub.
2. Download the latest **`SuperVelocityCurve-macOS-unsigned.zip`** (Mac) or **`SuperVelocityCurve-Windows-unsigned.zip`** (Windows).

> Builds are **unsigned** (free/open source). macOS may ask you to allow the plugin once — see [Troubleshooting](#troubleshooting) below.

## Step 2 — Unzip

Double-click the zip file. You should see folders like:

- `Super VelocityCurve.vst3`
- `Super VelocityCurve MIDI FX.vst3` (and/or `.component` on Mac)
- `Super VelocityCurve.app` (Mac standalone) or `.exe` (Windows standalone)

## Step 3 — Copy plugins to the right place

### macOS

| What you downloaded | Copy to |
|---------------------|---------|
| `Super VelocityCurve.vst3` | `~/Library/Audio/Plug-Ins/VST3/` |
| `Super VelocityCurve MIDI FX.vst3` | `~/Library/Audio/Plug-Ins/VST3/` |
| `Super VelocityCurve.component` | `~/Library/Audio/Plug-Ins/Components/` |
| `Super VelocityCurve MIDI FX.component` | `~/Library/Audio/Plug-Ins/Components/` |
| `Super VelocityCurve.app` | Applications folder (optional — runs without a DAW) |

**Finder shortcut:** In Finder, press **Cmd+Shift+G** and paste `~/Library/Audio/Plug-Ins/VST3/`.

### Windows

| What you downloaded | Copy to |
|---------------------|---------|
| `Super VelocityCurve.vst3` | `C:\Program Files\Common Files\VST3\` |
| `Super VelocityCurve MIDI FX.vst3` | `C:\Program Files\Common Files\VST3\` |
| `Super VelocityCurve.exe` | Any folder you like (standalone) |

## Step 4 — Rescan in your DAW

Open your DAW’s plugin preferences and **rescan VST3** (and AU on Mac).

- **Ableton Live:** use the **Instrument** build only — see [Getting started](getting-started.md#ableton-live).
- **Logic Pro:** use the **MIDI FX** build in the MIDI FX slot — see [Getting started](getting-started.md#logic-pro).

## Troubleshooting

### macOS: “plugin cannot be opened” / security warning

In Terminal, run (adjust the path if needed):

```bash
xattr -cr ~/Library/Audio/Plug-Ins/VST3/Super\ VelocityCurve.vst3
xattr -cr ~/Library/Audio/Plug-Ins/VST3/Super\ VelocityCurve\ MIDI\ FX.vst3
```

Or: **System Settings → Privacy & Security → Open Anyway** after the first blocked load.

### Windows: plugin doesn’t appear

Confirm the `.vst3` folder (not just files inside it) is in `Common Files\VST3\`, then rescan in your DAW.

### Still stuck?

Open a [GitHub Issue](https://github.com/azazhang/Super_VelocityCurve/issues) and say your OS, DAW, and which plugin variant you installed.

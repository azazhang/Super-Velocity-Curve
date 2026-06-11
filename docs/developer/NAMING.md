# Naming layers

One product, four intentional layers. Do not mix them in user-facing copy.

| Layer | Form | Where | Example |
|-------|------|-------|---------|
| **Brand** | Super Velocity Curve | README, About, docs, `PRODUCT_NAME`, `COMPANY_NAME` | Plugin window title |
| **Code / CMake** | `SuperVelocityCurve` | C++ classes, CMake targets, `project()`, ValueTree XML types | `SuperVelocityCurveAudioProcessor` |
| **Repo slug** | `Super_VelocityCurve` | GitHub URL only | `github.com/azazhang/Super_VelocityCurve` |
| **Release artefacts** | `SuperVelocityCurve-*` | CI zip filenames (no spaces — tooling) | `SuperVelocityCurve-macOS-unsigned.zip` |

## Rules

- **Never** use the hybrid `Super VelocityCurve` (two words + glued “Curve”) in user-facing text.
- **Do not rename** the GitHub repo or ValueTree type strings (`SuperVelocityCurveState`, etc.) without a migration plan — saved DAW projects and `.svcp` files depend on them.
- **Bundle IDs** (`com.supervelocitycurve.*`) and **plugin codes** (`Svcv`, `Svc2`) stay stable so hosts recognise upgrades.

## Repo rename

We keep **`Super_VelocityCurve`** as the repository slug. Renaming to `Super-Velocity-Curve` would require every clone to update `git remote`, and only partially redirects old links. The slug is a developer identifier; musicians see **Super Velocity Curve** in the DAW.

## Check

```bash
./scripts/check-branding.sh
```

# Naming layers

One product, four intentional layers. Do not mix them in user-facing copy.

| Layer | Form | Where | Example |
|-------|------|-------|---------|
| **Brand** | Super Velocity Curve | README, About, docs, `PRODUCT_NAME`, `COMPANY_NAME` | Plugin window title |
| **Code / CMake** | `SuperVelocityCurve` | C++ classes, CMake targets, `project()`, ValueTree XML types | `SuperVelocityCurveAudioProcessor` |
| **Repo slug** | `Super-Velocity-Curve` *(planned)* | GitHub URL | `github.com/azazhang/Super-Velocity-Curve` |
| **Release artefacts** | `SuperVelocityCurve-*` | CI zip filenames (no spaces — tooling) | `SuperVelocityCurve-macOS-unsigned.zip` |

Current slug **`Super_VelocityCurve`** is legacy (underscore + glued word). **Rename plan:** [REPO_RENAME.md](REPO_RENAME.md).

## Rules

- **Never** use the hybrid `Super VelocityCurve` (two words + glued “Curve”) in user-facing text.
- **Do not rename** ValueTree type strings (`SuperVelocityCurveState`, etc.) without a migration plan — saved DAW projects and `.svcp` files depend on them.
- **Bundle IDs** (`com.supervelocitycurve.*`) and **plugin codes** (`Svcv`, `Svc2`) stay stable so hosts recognise upgrades.

## Check

```bash
./scripts/check-branding.sh
```

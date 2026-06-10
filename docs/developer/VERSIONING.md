# Versioning policy

Super VelocityCurve uses **0.x.y** while the product is still in active beta:

| Segment | Meaning |
|---------|---------|
| **0** | Pre-1.0 — breaking changes and UX regressions are still expected |
| **x** | User-visible milestone (workflow, docs, host coverage) |
| **y** | Bugfix / polish within a milestone |

We **reset from 0.6.x → 0.2.0** (2026-06-09) because patch numbers had outpaced real stability. **Current version is in `CMakeLists.txt` only** — if `BACKLOG.md` or old changelog sections mention 0.6.x, treat them as historical. **1.0.0** is reserved for: pluginval-clean matrix, GitHub release zips, and manual DAW smoke on target hosts without known P0 UI bugs.

Do not tag a GitHub release until [RELEASE.md](RELEASE.md) checklist passes.

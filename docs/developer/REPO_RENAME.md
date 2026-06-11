# Repository rename

**Status: completed 2026-06-11** — `azazhang/Super_VelocityCurve` → **`azazhang/Super-Velocity-Curve`**

Historical plan for moving from the awkward hybrid slug to a URL that matches the public brand **Super Velocity Curve**.

## Recommended new slug

| Candidate | Verdict |
|-----------|---------|
| **`Super-Velocity-Curve`** | **Recommended** — three words, hyphen-separated; matches brand; readable in the address bar |
| `super-velocity-curve` | Valid GitHub convention (lowercase); less visible brand match |
| `SuperVelocityCurve` | Rejects the glued-word problem we just fixed in the UI |
| `Super_Velocity_Curve` | Still awkward (underscores + partial glue) |

**Target URL:** `https://github.com/azazhang/Super-Velocity-Curve`

Release zip names (`SuperVelocityCurve-macOS-unsigned.zip`) stay as-is for now — they are artefact IDs, not the public repo link. Renaming zips is optional in a later release.

---

## What GitHub does automatically

Per [GitHub Docs — Renaming a repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/renaming-a-repository):

| Redirected | Notes |
|------------|-------|
| Web UI (repo, issues, PRs, releases, tags) | Permanent while old name is unused |
| `git clone` / `fetch` / `push` to old URL | Works transparently |
| Stars, watchers, forks metadata | Preserved |

| **Not** redirected | Impact on this project |
|---------------------|------------------------|
| **GitHub Pages** project URLs | **None** — we do not use Pages (`has_pages: false`) |
| **Reusable Actions** (`uses: owner/repo@ref`) | **None** — we do not publish actions for other repos |
| Local `git remote` URL | Stale until updated; `git` still works via redirect |
| `gh` in scripts | May behave oddly with old remote ([cli#11754](https://github.com/cli/cli/issues/11754)) |

**Critical warning:** Never create a *new* repo named `Super_VelocityCurve` after the rename — that breaks redirects to the new home.

---

## Pre-flight checklist

- [ ] Confirm slug `Super-Velocity-Curve` is available under `azazhang` (rename fails if taken)
- [ ] No open release pipeline running
- [ ] Note any external links you control (homepage, Ko-fi, YouTube, forum posts)

### In-repo references to update (6 files today)

| File | What |
|------|------|
| `Source/Config/AppUrls.h` | `kGitHubRepo` — About panel GitHub button |
| `README.md` | Releases link |
| `docs/user/install.md` | Releases + Issues links |
| `docs/developer/WINDOWS_TESTING.md` | Actions link |
| `scripts/watch-ci.sh` | `GITHUB_REPOSITORY` default |
| `docs/developer/NAMING.md` | Slug table + repo section |

Search before merge:

```bash
rg 'Super_VelocityCurve' --glob '!JUCE' --glob '!third_party'
```

### Does **not** change

| Layer | Stays |
|-------|-------|
| C++ / CMake targets | `SuperVelocityCurve`, `SuperVelocityCurveMidiFx` |
| ValueTree XML types | `SuperVelocityCurveState`, etc. |
| Bundle IDs | `com.supervelocitycurve.*` |
| iCloud / local clone folder | Optional manual rename; git does not care |

---

## Execution order (maintenance window ~15 min)

### Phase A — You (GitHub admin)

1. **Settings → General → Repository name** → `Super-Velocity-Curve` → **Rename**  
   Or from a clone with admin rights:
   ```bash
   gh repo rename Super-Velocity-Curve -R azazhang/Super_VelocityCurve -y
   ```
2. Verify redirect: open `https://github.com/azazhang/Super_VelocityCurve` — should land on new URL.

### Phase B — Codebase (commit immediately after Phase A)

1. Replace all `azazhang/Super_VelocityCurve` → `azazhang/Super-Velocity-Curve` in the six files above.
2. Update `docs/developer/NAMING.md` slug row.
3. Run `./scripts/check-branding.sh`.
4. Commit: `chore: update repo URLs after rename to Super-Velocity-Curve`
5. Push to `main`.

### Phase C — Every clone (including this machine)

```bash
git remote set-url origin https://github.com/azazhang/Super-Velocity-Curve.git
git remote -v   # confirm
git fetch origin
```

Optional local folder rename (cosmetic):

```bash
# from parent directory — adjust path to your machine
mv Super_VelocityCurve Super-Velocity-Curve
```

Re-open the project in Cursor from the new folder if you rename.

### Phase D — Validation

| Check | Command / action |
|-------|----------------|
| Old URL redirects | Browser: `github.com/azazhang/Super_VelocityCurve/releases` |
| New URL works | `github.com/azazhang/Super-Velocity-Curve/releases/latest` |
| About panel link | Build Standalone → About → GitHub |
| CI | Push triggers Build workflow green |
| `gh` | `gh run list -R azazhang/Super-Velocity-Curve` |
| Branding | `./scripts/check-branding.sh` |

### Phase E — External (manual)

- [ ] `azhang.eu.org` homepage link if it points at the old slug
- [ ] Ko-fi / social bios
- [ ] Any README mirrors or bookmarks you shared

No GitHub Packages or Pages to migrate.

---

## Rollback

GitHub allows renaming again (e.g. back to `Super_VelocityCurve`) if the name is still free. Revert the URL commit and update remotes. Avoid leaving both names in use.

---

## Optional follow-ups (not required for rename)

- Release zip prefix `Super-Velocity-Curve-*` (breaking for download scripts; do with a tagged release + changelog note)
- Add `kGitHubRepoSlug` next to `kGitHubRepo` in `AppUrls.h` for scripts that parse owner/repo
- README badge: `![Build](https://github.com/azazhang/Super-Velocity-Curve/actions/workflows/build.yml/badge.svg)`

---

## Why rename now

`Super_VelocityCurve` is the last “awkward middle” visitors see — underscore plus glued `VelocityCurve` does not match **Super Velocity Curve** in the DAW or README. Aligning the slug removes cognitive friction for musicians who land from Releases or Issues.

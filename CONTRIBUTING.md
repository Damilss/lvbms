# Contributing to lvbms

Thanks for helping out! This guide covers how we branch, commit, and merge.
If anything here is unclear, ask a firmware lead before guessing.

## TL;DR

1. Branch off `main`: `git switch main && git pull && git switch -c feat/short-name`
2. Commit using [Conventional Commits](#commit-messages): `feat(bq769): read cell voltages`
3. Push your branch and open a Pull Request into `main`
4. Get at least one review, then **merge** (no squash, no rebase)

## Branching

**Never commit directly to `main`.** All work happens on a branch and comes in
through a Pull Request. The only exception is a repo lead doing repo
maintenance who knows exactly what they're doing.

Name branches `<type>/<short-description>`, using the same types as commits:

| Example                     | For                              |
| --------------------------- | -------------------------------- |
| `feat/bq769-cell-voltages`  | new functionality                |
| `fix/i2c-timeout`           | bug fixes                        |
| `docs/contributing`         | documentation only               |
| `refactor/taskmanager`      | restructuring, no behavior change |
| `chore/gitignore`           | tooling, config, housekeeping    |

Keep branches short-lived and focused on one thing. If your branch falls
behind `main`, bring it up to date with a merge:

```sh
git switch main && git pull
git switch feat/my-branch
git merge main
```

## Commit messages

We use [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<optional scope>): <short summary>

<optional body: what changed and why>
```

**Types**

| Type       | Use it when you...                                   |
| ---------- | ---------------------------------------------------- |
| `feat`     | add a new feature or driver function                 |
| `fix`      | fix a bug                                            |
| `docs`     | change documentation only                            |
| `refactor` | restructure code without changing behavior           |
| `test`     | add or update tests                                  |
| `chore`    | change build config, tooling, repo setup, etc.       |

**Scope** (optional) is the area you touched, e.g. `bq769`, `i2c`, `can`,
`rtos`, `lvbattery`, `lvm`.

**Rules**

- Summary is imperative and lowercase: `add`, `fix`, `remove` (not `added`, `fixes`)
- Keep the summary under ~72 characters, no trailing period
- One logical change per commit; don't mix a refactor with a bug fix
- Use the body to explain *why* when it isn't obvious

**Examples**

```
feat(bq769): add cell voltage read over I2C
fix(can): correct DLC for battery status frame
docs: add wiring notes for LVBattery board
refactor(taskmanager): split init into per-task functions
chore: add .gitignore for build artifacts
```

## Pull Requests

- Open PRs into `main` and fill out the PR template
- Keep PRs small enough to review in one sitting
- Make sure it **builds** before requesting review; if you tested on hardware, say which board and what you checked
- At least **one approval** is required before merging
- Address review comments with new commits (don't force-push over review history)

### Merging

**We use merge commits only.** Do not use "Squash and merge" or "Rebase and
merge". This keeps the full commit history of each branch, so write commits
you're happy to keep.

After merging, delete the branch.

## Things not to commit

- Build output (`Debug/`, `Release/`, `build/`, `*.elf`, `*.bin`, `*.hex`, `*.o`)
- Personal IDE settings
- Secrets, credentials, or large binary files (link to datasheets instead of committing them)

The `.gitignore` covers the common cases. If you see something unexpected in
`git status`, ask before committing it.

## Code style

- Match the style of the surrounding code
- Put board-specific code under `boards/<BoardName>/` and anything reused across boards under `shared/`
- Don't hand-edit vendor/generated HAL files unless you have to; note it in your PR if you do

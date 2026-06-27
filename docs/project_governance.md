# Project governance

## Scope

This project uses two issue/report layers:

- `issues/` and `reports/` at the repository root for project-wide work items, audits, and cross-module decisions.
- `modules/<NN>_<name>/issues/` and `modules/<NN>_<name>/reports/` for module-owned execution details.

Each module still has local docs under `modules/<NN>_<name>/docs/`, but project-level coordination decisions belong in the root issue and report indexes unless the topic is module-local.

## Core operating rules

### Issue-first

- Create or locate an issue document before any implementation work starts.
- Start work only after that issue is reflected in both:
  - `issues/README.md` (project scope), or the module issue index (module scope).
  - The module or project issue tracker table row.

### Branch-first

- Every implementation issue uses the branch/worktree recorded in the owning issue file.
- Development should branch from `develop` and work in the assigned worktree.
- Keep issue-scoped work grouped to the branch and worktree for parallel work safety.

### TDD-first

- Record TDD evidence in the issue before implementation.
- A red step (expected fail) is required for planned behavior changes, unless the issue is explicitly planning-only.
- Use issue test records (command, expected result, and actual failure) from the issue template before implementation.

## Report governance

- Report names must use: `YYYYMMDD_HHMMSS_<topic>.md` (UTC timestamp + short topic slug).
- Root reports are for project-level audits and milestones.
- Module reports are for module-owned decisions, validation summaries, and module-local artifacts.
- Root and module report indexes must reference generated files after each report is added.

## Documenting status

- Root issue workflow and project/module indexes are updated as work progresses.
- Module and project issue/report tables are the source of truth for scope and completion visibility.
- This policy is shared with all modules so each module README index can remain discoverable and consistent.

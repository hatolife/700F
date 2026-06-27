# Issue workflow

## Working rule

All implementation tasks follow **issue-first**, **branch-first**, and **TDD-first**:

- **Issue-first:** create or select the governing issue file before touching code.
- **Branch-first:** use the branch/worktree named in that issue (usually a path under `docs/`, `research/`, or `feature/`).
- **TDD-first:** record a failing test/expected-failure check in the issue before implementation.

## Step sequence

1. Identify scope (`project` or module) from the issue owner field and branch plan.
2. Update the relevant tracker README index before work:
   - Project scope: `issues/README.md`.
   - Module scope: `modules/<NN>_<name>/issues/README.md`.
3. Record TDD evidence in the issue:
   - red command and expected failure
   - expected failure output summary
4. Implement only after issue evidence exists.
5. Document outputs and outcomes in `reports/` or `modules/<NN>_<name>/reports/` with `docs/reporting_policy.md`.
6. Run local checks (`./tools/run_ci_local.sh`) before handoff.

## Scope separation

- **Root issues** (`issues/README.md`): cross-module decisions, milestones, audits, and project-wide policy work.
- **Module issues** (`modules/<NN>_<name>/issues/README.md`): local module implementation work tied to module owners.

## Report timing

Issue progress is reported when any of these happen:

- after a major decision is finalized,
- after any audit/check result,
- when implementation milestones complete for the issue,
- before handoff.

## Required links

- [Project governance](project_governance.md)
- [Reporting policy](reporting_policy.md)
- [Versioning policy](versioning_policy.md)

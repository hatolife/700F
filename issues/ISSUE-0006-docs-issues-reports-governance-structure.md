# ISSUE-0006: Docs issues reports governance structure

## Status
open

## Scope
docs

## Priority
P0

## Owner
- Codex role: Manager + all modules
- Recommended model: GPT-5.3-Codex-Spark

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0006-governance-structure
- Worktree: ../f700f-wt-issue-0006
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; documentation only module versions as needed
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Standardize project/module docs, issues, and reports structure for issue-first operation.

## Affected paths
- docs/project_governance.md
- docs/issue_workflow.md
- docs/reporting_policy.md
- docs/versioning_policy.md
- issues/README.md
- reports/README.md
- modules/*/docs/README.md
- modules/*/issues/README.md
- modules/*/reports/README.md

## Dependencies
- ISSUE-0004

## Acceptance criteria
- [x] Project issues vs module issues are clearly distinguished.
- [x] Report creation timing is documented.
- [x] Report filename format `YYYYMMDD_HHMMSS_<topic>.md` is documented.
- [x] All modules have docs/issues/reports directories and README indexes.

## Test plan
- python3 tools/check_governance.py

## TDD record
- Red test added: `bash -lc "test -f docs/project_governance.md -a -f docs/reporting_policy.md -a -f docs/versioning_policy.md"`.
- Red command: `bash -lc "test -f docs/project_governance.md -a -f docs/reporting_policy.md -a -f docs/versioning_policy.md"`.
- Expected failure: command fails before implementation because the three governance policy files do not exist.
- Green command: `./tools/run_ci_local.sh`.
- Refactor notes: no refactor; docs and index normalization only.
- TDD exception if any: planning-only exception until implementation begins.

### Red evidence
- 2026-06-27 00:00 UTC: command failed with `missing docs/project_governance.md`.

### Validation checks
- 2026-06-27 00:01 UTC Local CI passed: bash ./tools/run_ci_local.sh (all checks passed).

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.

## Result
Completed: root and module docs/issues/reports governance indexes updated; CI passed.


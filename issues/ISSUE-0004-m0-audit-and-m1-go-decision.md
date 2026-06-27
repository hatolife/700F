# ISSUE-0004: M0 audit and M1 go decision

## Status
open

## Scope
qa

## Priority
P0

## Owner
- Codex role: Manager / QA
- Recommended model: GPT-5.3-Codex-Spark; escalate to gpt-5.5-low on mismatch

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0004-m0-audit
- Worktree: ../f700f-wt-issue-0004
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: no immediate bump; audit gates M1 start
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Audit ISSUE-0001/M0 artifacts and decide whether the repository is ready to proceed to M1.

## Affected paths
- reports/YYYYMMDD_HHMMSS_m0_audit.md
- issues/README.md

## Dependencies
- ISSUE-0001, ISSUE-0002

## Acceptance criteria
- [ ] `./tools/run_ci_local.sh` succeeds.
- [ ] `git status` is clean at audit start and finish.
- [ ] Exactly 16 module skeletons exist.
- [ ] Each module has AGENTS.md.
- [ ] version manifest records project and all modules as v0.0.1 for M0 baseline.
- [ ] Audit report is saved under reports with `YYYYMMDD_HHMMSS_m0_audit.md` format.

## Test plan
- ./tools/run_ci_local.sh
- git status --short
- python3 tools/check_versions.py
- python3 tools/check_governance.py

## TDD record
- Red test added: to be completed by the issue owner before implementation.
- Red command: to be completed by the issue owner.
- Expected failure: to be completed by the issue owner.
- Green command: see Test plan.
- Refactor notes: to be completed by the issue owner.
- TDD exception if any: planning-only issue until implementation begins.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.

## Result
Pending.

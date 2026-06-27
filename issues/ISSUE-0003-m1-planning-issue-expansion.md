# ISSUE-0003: M1 planning issue expansion

## Status
closed

## Scope
project

## Priority
P0

## Owner
- Codex role: Manager
- Recommended model: GPT-5.5

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0003-m1-planning
- Worktree: ../f700f-wt-issue-0003
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project v0.1.0 target planning only
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create the M1 milestone plan, root issues, module issue stubs, and target version manifest entries before implementation starts.

## Affected paths
- issues/
- docs/milestones/m1_baseline_simulation.md
- versions.toml

## Dependencies
- ISSUE-0001, ISSUE-0002

## Acceptance criteria
- [ ] All requested M1 root issues exist.
- [ ] issues/README.md lists all new issues.
- [ ] Relevant module issue stubs exist.
- [ ] versions.toml records M1 target project version v0.1.0.
- [ ] local CI passes.

## Test plan
- ./tools/run_ci_local.sh

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
M1 planning issue expansion completed in this planning branch.

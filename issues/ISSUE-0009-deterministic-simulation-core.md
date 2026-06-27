# ISSUE-0009: Deterministic simulation core

## Status
open

## Scope
simulation

## Priority
P0

## Owner
- Codex role: Module 11 + Module 02 + Module 12
- Recommended model: gpt-5.5-medium

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0009-deterministic-simulation-core
- Worktree: ../f700f-wt-issue-0009
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 11, 02, 12 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create deterministic seed-fixed simulation runner and mode -> channel -> receiver -> metrics pipeline.

## Affected paths
- modules/11_pipeline_runner/
- docs/specs/simulation_pipeline.md
- tests/integration/

## Dependencies
- ISSUE-0008

## Acceptance criteria
- [ ] Same config and seed produce same result.
- [ ] Mode can be swapped.
- [ ] Channel can be swapped.
- [ ] Metrics can be swapped.
- [ ] Minimal config can run one simulation.
- [ ] Result can be emitted as JSON or CSV.

## Test plan
- ctest --test-dir build --output-on-failure

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

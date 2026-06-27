# ISSUE-0015: Sweep runner v0.1.0

## Status
open

## Scope
module:13

## Priority
P0

## Owner
- Codex role: Module 13 + Module 11 + Module 15
- Recommended model: gpt-5.5-medium

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0015-sweep-runner-v0-1
- Worktree: ../f700f-wt-issue-0015
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 13, 11, 15 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create config-driven sweep runner for modes, channel conditions, and seeds.

## Affected paths
- modules/13_sweep_optimization/
- configs/sweeps/m1_baseline.toml
- docs/specs/sweep_runner.md

## Dependencies
- ISSUE-0009, ISSUE-0014

## Acceptance criteria
- [ ] SSB standard, SSB narrow, 700D official-or-stub, and 700E official-or-stub can be listed in one sweep config.
- [ ] Results are saved under reports or artifacts.
- [ ] Seed-fixed runs are reproducible.
- [ ] Codec2-disabled environment skips official 700D/700E with skip reason.
- [ ] Short CI sweep exists.

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

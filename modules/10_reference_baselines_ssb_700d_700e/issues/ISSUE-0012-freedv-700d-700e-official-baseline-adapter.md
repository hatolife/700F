# ISSUE-0012: FreeDV 700D 700E official baseline adapter

## Status
open

## Scope
module:10_reference_baselines_ssb_700d_700e

## Priority
P0

## Owner
- Codex role: Module 10
- Root tracking issue: ISSUE-0012

## Branch
- Base branch: develop
- Work branch: see root ISSUE-0012
- Worktree: ../f700f-wt-issue-0012
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target
- Module version: v0.1.0 target if implementation touches this module
- Compatibility line: 1
- Milestone: M1 Baseline Simulation

## Task
Module-local planning and implementation handoff for root ISSUE-0012: FreeDV 700D 700E official baseline adapter.

## Acceptance criteria
- [x] Root ISSUE-0012 acceptance criteria relevant to this module are satisfied: official
  descriptors are distinguishable from existing emulator descriptors.
- [x] Module docs/issues/reports indexes are updated as work proceeds.
- [x] Module tests or documented TDD exception are added before implementation handoff closes.

## Test plan
- ./tools/run_ci_local.sh
- TDD red test before implementation: add `test_freedv_official_adapter` to prove
  official 700D/700E descriptors are distinguishable from the existing
  `freedv700d_emulated` and `freedv700e_emulated` descriptors.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 issue expansion.
- 2026-06-28 JST M1-C subagent B confirmed existing Module 10 emulator descriptors
  remain descriptor-only and will keep `emulator=true`, `official_baseline=false`.
- 2026-06-28 JST `test_freedv_official_adapter` verifies official descriptors have
  `official_baseline=true`, `emulator=false`, and different ids from Module 10 emulator
  descriptors without changing Module 10 runtime code.

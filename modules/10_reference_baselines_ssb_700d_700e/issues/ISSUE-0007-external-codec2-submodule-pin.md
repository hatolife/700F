# ISSUE-0007: external codec2 submodule pin

## Status
open

## Scope
module:10_reference_baselines_ssb_700d_700e

## Priority
P0

## Owner
- Codex role: Module 10
- Root tracking issue: ISSUE-0007

## Branch
- Base branch: develop
- Work branch: see root ISSUE-0007
- Worktree: ../f700f-wt-issue-0007
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target
- Module version: v0.1.0 target if implementation touches this module
- Compatibility line: 1
- Milestone: M1 Baseline Simulation

## Task
Module-local planning and implementation handoff for root ISSUE-0007: external codec2 submodule pin.

## Acceptance criteria
- [x] Root ISSUE-0007 acceptance criteria relevant to this module are satisfied.
- [x] Module docs/issues/reports indexes are updated as work proceeds.
- [x] Module tests or documented TDD exception are added before implementation handoff closes.

## Test plan
- ./tools/run_ci_local.sh

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 issue expansion.
- 2026-06-27 14:40 UTC Subagent D recorded root TDD evidence before implementation; Module 10 scope is source/pin documentation for later baseline work, without implementing SSB/700D/700E modes.
- 2026-06-27 15:20 UTC Root dependency check and local CI passed. Module 10 receives pinned 700D/700E source anchors in `docs/codec2_700d_700e_verification.md`; baseline implementation remains deferred.

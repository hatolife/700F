# ISSUE-0010: Channel model v0.1.0

## Status
done

## Scope
module:08_hf_channel_simulator

## Priority
P0

## Owner
- Codex role: Module 08
- Root tracking issue: ISSUE-0010

## Branch
- Base branch: develop
- Work branch: see root ISSUE-0010
- Worktree: ../f700f-wt-issue-0010
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target
- Module version: v0.1.0 target if implementation touches this module
- Compatibility line: 1
- Milestone: M1 Baseline Simulation

## Task
Module-local planning and implementation handoff for root ISSUE-0010: Channel model v0.1.0.

## TDD record
- Red test file: `tests/test_channel_models.cpp` before implementation.
- Test focus:
  - AWGN deterministic behavior with fixed and varying seeds (complex and audio paths)
  - SNR monotonicity check on noise power
  - Frequency-offset zero-Hz no-op check and invalid sample-rate rejection
  - Simple gain fading deterministic output with fixed seed
  - Channel chain order preservation
  - Empty-block handling on audio and complex paths

## Acceptance criteria
- [x] Root ISSUE-0010 acceptance criteria relevant to this module are satisfied.
- [x] Module docs/issues/reports indexes are updated as work proceeds.
- [x] Module tests or documented TDD exception are added before implementation handoff closes.

## Work log
- 2026-06-27 00:00 UTC Updated by Manager Codex with required TDD plan before first implementation change.

## Test plan
- ./tools/run_ci_local.sh

## Test evidence
- 2026-06-27: Added `tests/test_channel_models.cpp` and validated deterministic/channel-chain behavior in local TDD loop.
- 2026-06-27: `bash ./tools/run_ci_local.sh` completed successfully (channel model contract test passed).

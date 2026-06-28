# ISSUE-0025: M2 expanded channel condition matrix

## Status
closed

## Scope
module:08

## Priority
P0

## Owner
- Codex role: Module 08 + Module 13
- Recommended model: GPT-5.3-Codex-Spark; gpt-5.5-medium for channel edge cases

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0025-expanded-channel-matrix
- Worktree: ../f700f-wt-issue-0025
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 08 and 13 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018, ISSUE-0010

## Task
Define an M2 channel condition matrix using existing channel model primitives.

## Acceptance criteria
- [x] Smoke and full channel matrices are documented.
- [x] Smoke matrix includes identity, AWGN 6 dB, AWGN 0 dB, seed 1.
- [x] Full matrix includes AWGN SNR -2, 0, 2, 4, 6, 8 dB; frequency offsets 0, 50, 100, 200 Hz; simple fading none, weak, medium; seeds 1, 2, 3.
- [x] Condition ids are unique.
- [x] Invalid SNR/frequency offset and empty seed list are rejected.
- [x] CI smoke subset and full subset are distinguishable.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added before implementation in `tests/test_sweep_runner.cpp` for:
  smoke/full M2 matrix helper shape, duplicate condition ids, invalid SNR/frequency
  offset parameters, and empty seed lists.
- Initial targeted build failed as expected with unresolved
  `make_m2_channel_matrix_*_sweep_config` helper references before implementation.

## Result
Implemented. Local CI passed on 2026-06-28 with 10/10 CTest tests plus version and
governance checks.

## Implementation notes
- Added `docs/specs/channel_condition_matrix.md` and TOML-friendly matrix shapes under
  `configs/channels/`.
- Added Module 13 helpers:
  - `make_m2_channel_matrix_smoke_sweep_config()`
  - `make_m2_channel_matrix_full_sweep_config()`
- Preserved Module 08 channel primitives as source of truth; validation added only at
  the sweep orchestration/config-adapter boundary.
- ISSUE-0026 remains responsible for campaign-specific mode selection and report cadence.

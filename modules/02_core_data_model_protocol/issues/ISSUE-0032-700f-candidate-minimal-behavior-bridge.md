# ISSUE-0032: 700F candidate minimal behavior bridge

## Status
complete

## Scope
module:02

## Branch
- Work branch: feature/ISSUE-0032-700f-candidate-minimal-behavior

## Task
Define any data/protocol boundary needed for clearly labeled 700F surrogate
minimal behavior.

## Acceptance criteria
- [x] Surrogate and not-downselect-valid labels are represented clearly.
- [x] Interface changes are documented before implementation.

## TDD record
- 2026-06-29: Starting with failing tests for explicit surrogate status,
  `not_real_modem`, `downselect_valid = false`, `performance_valid = false`,
  surrogate model metadata, synthetic metric labeling, and real-performance
  score separation.
- 2026-06-29: `bash ./tools/run_ci_local.sh` passed after implementation.

## Result
ISSUE-0032 surrogate boundary is represented through append-only metrics/reporting
fields and documented in `docs/specs/700f_candidate_surrogate_behavior.md`.

# ISSUE-0047: occupied bandwidth validation harness

- Status: complete
- Priority: P0
- Scope: module:12,15,07,13
- Owner: Module 12 + Module 15 + Module 07 + Module 13
- Branch: feature/ISSUE-0047-occupied-bandwidth-validation
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0046
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

700F-A and future OFDM frames need a repeatable simulation-side occupied
bandwidth estimate before the project can compare bandwidth behavior in reports.

## Acceptance Criteria

- [x] Add a deterministic bandwidth measurement utility or harness.
- [x] Handle empty and zero signals safely.
- [x] Reject invalid sample rates.
- [x] Provide stable estimates for single-tone and two-tone test inputs.
- [x] Record occupied bandwidth estimate fields in result/report paths.
- [x] Document measurement limitations and 1.9 kHz target comparison.
- [x] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planned red tests: zero signal handling, single-tone estimate stability,
  two-tone wider-than-single-tone estimate, invalid sample-rate rejection, empty
  input safety, artifact/report field propagation.
- 2026-06-29: Started implementation on
  `feature/ISSUE-0047-occupied-bandwidth-validation` with planned red tests for
  deterministic FFT/energy-threshold measurement safety, tone-width ordering,
  result artifact propagation, and Markdown report visibility.
- 2026-06-29: Implemented deterministic DFT/energy-ratio occupied bandwidth
  harness, artifact/report/sweep propagation, and documentation. Focused tests
  passed, default local CI passed with 22/22 CTest plus version/governance
  checks, and Codec2 ON local CI passed with 22/22 CTest, 27 completed smoke
  rows, and 6 validated official FreeDV rows. Codec2 ON required explicit WSL
  `GIT_DIR`, `GIT_COMMON_DIR`, and `GIT_WORK_TREE` because this sibling
  worktree stores a Windows-style gitdir pointer.

## Notes

This is not a regulatory-grade occupied bandwidth measurement. It is an M3-A
simulation harness for future comparison and regression.

Completed implementation preserves the real 700F downselect prohibition:
prototype rows remain `performance_valid=false`, `performance_validity=limited`,
`downselect_valid=false`, and `downselect_validity=invalid`.

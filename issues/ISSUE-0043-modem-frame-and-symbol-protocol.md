# ISSUE-0043: modem frame and symbol protocol

- Status: complete
- Priority: P0
- Scope: module:02,07,11,13
- Owner: Module 02 + Module 07 + Module 11 + Module 13
- Branch: feature/ISSUE-0043-modem-frame-symbol-protocol
- Target version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up
- Depends: ISSUE-0041
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

ISSUE-0041 defines RFC-level modem boundary objects. The repository needs the
minimal source-level protocol representation before ISSUE-0042 implements the
QPSK-like prototype.

## Acceptance Criteria

- [x] Add source-level equivalents for `ModemInputFrame`, `ModemOutputFrame`,
  `SymbolBlock`, `BasebandFrame`, and `PrototypeFrameStatus`.
- [x] Add contract tests for default construction, status/limitation metadata,
  and JSON/report-visible field propagation as appropriate.
- [x] Keep existing Mode interface compatibility.
- [x] Do not implement real waveform behavior.
- [x] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- 2026-06-29: Started with protocol contract tests for frame/symbol/baseband
  default construction, placeholder status metadata, rate validation, and
  zero-length frame safety before implementation.
- 2026-06-29: Red test confirmed missing ISSUE-0043 protocol types, then
  implementation added default-constructible value containers and rate
  validation helpers.

## Verification

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed with 21/21 CTest,
  version check, and governance check.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with 21/21
  CTest, 27/27 Codec2 ON smoke rows, and 6/6 official FreeDV rows validated.
  The wrapper was invoked from WSL with explicit Git worktree environment
  variables because the Windows worktree `.git` pointer is not directly
  readable by WSL Git.

## Notes

This issue should land before ISSUE-0042 where possible. ISSUE-0043 added
protocol containers only; 700F-A QPSK-like waveform implementation remains
owned by ISSUE-0042.

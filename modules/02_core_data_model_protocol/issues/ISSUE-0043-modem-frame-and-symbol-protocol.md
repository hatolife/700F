# ISSUE-0043: modem frame and symbol protocol

## Status
complete

## Scope
module:02

## Priority
P0

## Owner
- Codex role: Module 02 + Module 07 + Module 11 + Module 13

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0043-modem-frame-symbol-protocol
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up

## Task
Create the minimal source-level protocol for modem frames, symbols, baseband
frames, and prototype statuses.

## Acceptance criteria
- [x] Contract tests exist before implementation.
- [x] Protocol maps to ISSUE-0041 RFC names.
- [x] No real modem behavior is added.

## TDD record
- 2026-06-29: Started with protocol contract tests for frame/symbol/baseband
  default construction, placeholder status metadata, rate validation, and
  zero-length frame safety before implementation.
- 2026-06-29: Red test confirmed missing ISSUE-0043 protocol types, then
  implementation added the minimal value containers and validation helpers.

## Verification

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed with 21/21 CTest,
  version check, and governance check.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with 21/21
  CTest, 27/27 Codec2 ON smoke rows, and 6/6 official FreeDV rows validated.

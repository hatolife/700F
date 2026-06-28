# ISSUE-0043: modem frame and symbol protocol

- Status: open
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

- [ ] Add source-level equivalents for `ModemInputFrame`, `ModemOutputFrame`,
  `SymbolBlock`, `BasebandFrame`, and `PrototypeFrameStatus`.
- [ ] Add contract tests for default construction, status/limitation metadata,
  and JSON/report-visible field propagation as appropriate.
- [ ] Keep existing Mode interface compatibility.
- [ ] Do not implement real waveform behavior.
- [ ] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Pending. Start with protocol contract tests before implementation.

## Notes

This issue should land before ISSUE-0042 where possible.

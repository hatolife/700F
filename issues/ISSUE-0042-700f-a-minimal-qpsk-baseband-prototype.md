# ISSUE-0042: 700F-A minimal QPSK baseband prototype

- Status: open
- Priority: P0
- Scope: module:07,02,11,12,13,15
- Owner: Module 07 + Module 02 + Module 13 + Module 15
- Branch: feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype
- Target version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up
- Depends: ISSUE-0041, ISSUE-0043
- Recommended model: gpt-5.5-xhigh
- Last updated: 2026-06-29

## Problem

ISSUE-0041 selected Option B as the first implementation step after the toy
waveform prototype. 700F-A needs a minimal QPSK-like baseband prototype that uses
the RFC boundary metadata without claiming final modem performance.

## Acceptance Criteria

- [ ] Implement only `freedv700f_a_balanced` as
  `implementation_status = real_modem_prototype`.
- [ ] Preserve 700F-B and 700F-C as surrogate rows.
- [ ] Use ISSUE-0043 frame/symbol protocol boundaries when available.
- [ ] Emit `prototype = true`, `not_final_modem = true`,
  `downselect_valid = false`, and `performance_valid = limited`.
- [ ] Keep FEC/sync/codec placeholders explicit in metadata and reports.
- [ ] Keep Codec2 OFF local CI green.
- [ ] Keep Codec2 ON local CI green and preserve official 700D/700E rows.

## TDD Record

- Pending. Start with failing tests for metadata, B/C preservation, and minimal
  baseband frame/status behavior before implementation.

## Notes

Do not implement OFDM/FEC/sync final behavior here. Do not perform downselect.

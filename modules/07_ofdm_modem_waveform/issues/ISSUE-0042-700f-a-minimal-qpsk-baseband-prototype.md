# ISSUE-0042: 700F-A minimal QPSK baseband prototype

## Status
complete

## Scope
module:07

## Priority
P0

## Owner
- Codex role: Module 07 + Module 02 + Module 13 + Module 15

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up

## Task
Implement the minimal QPSK-like 700F-A baseband prototype authorized by
ISSUE-0041 after ISSUE-0043 supplies the protocol boundary.

## Acceptance criteria
- [x] 700F-A uses real prototype metadata.
- [x] 700F-B/C remain surrogate.
- [x] FEC/sync/codec gaps are explicit limitations.
- [x] OFF and ON CI pass.

## TDD record
- 2026-06-29: Subagent C started with red tests planned for 700F-A
  `real_modem_prototype` metadata, ISSUE-0043 `SymbolBlock` and
  `BasebandFrame` generation, invalid-rate rejection, zero-length safety,
  identity/AWGN smoke completion, report guardrails, and 700F-B/C surrogate
  preservation.
- 2026-06-29: Red tests first exposed missing minimal QPSK helper behavior and
  incomplete metadata propagation. Implementation added the guarded
  SymbolBlock/BasebandFrame path for 700F-A only.
- 2026-06-29: M2-H smoke/report, local Codec2-OFF CI, and local Codec2-ON CI
  passed. Codec2 ON preserved the six official 700D/700E rows.

## Result

Completed. Module 07 now supplies the 700F-A minimal QPSK-like baseband
prototype path and leaves 700F-B/C as surrogate descriptors. The result remains
non-final, downselect-invalid, and performance-limited with explicit
`codec_family=synthetic`, `fec_family=none`, and `sync_family=none`.

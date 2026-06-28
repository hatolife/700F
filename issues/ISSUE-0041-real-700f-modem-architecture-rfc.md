# ISSUE-0041: real 700F modem architecture RFC

- Status: complete
- Priority: P0
- Scope: module:02,07,11,12,13,15
- Owner: Module 02 + Module 07 + Module 13 + Module 15
- Branch: docs/ISSUE-0041-real-700f-modem-architecture-rfc
- Target version: v0.2.0 target
- Milestone: M2 real modem RFC
- Depends: ISSUE-0040
- Last updated: 2026-06-29

## Problem

ISSUE-0040 authorizes design work for the step after the 700F-A toy waveform
prototype. The repository needs an RFC that defines the boundary between
`toy_audio_waveform` and a real modem prototype before any implementation issue
starts changing waveform, FEC, sync, codec, metrics, or reporting semantics.

## Acceptance Criteria

- [x] Define the real 700F modem architecture RFC in
  `docs/specs/700f_real_modem_architecture_rfc.md`.
- [x] Define data/protocol boundaries in
  `docs/specs/700f_modem_boundaries.md`.
- [x] Evaluate architecture options A-D and record the recommended path.
- [x] Require explicit prototype metadata:
  `implementation_status = real_modem_prototype`, `prototype = true`,
  `not_final_modem = true`, `downselect_valid = false`,
  `performance_valid = limited`, `modem_family`, `fec_family`, `sync_family`,
  `codec_family`, and `limitations`.
- [x] Define the required boundary objects: `ModemInputFrame`,
  `ModemOutputFrame`, `SymbolBlock`, `BasebandFrame`, and
  `PrototypeFrameStatus`.
- [x] Keep real downselect prohibited until a later audit authorizes
  performance-valid evidence.
- [x] Local Codec2 OFF and Codec2 ON CI pass.

## TDD Record

- TDD exception: RFC/design-only issue. No real modem implementation is added.
- 2026-06-29: Started from `develop` after ISSUE-0040 merge. ISSUE-0040
  integration verification passed default Codec2 OFF CI and Codec2 ON CI.

## Result

Completed. The selected architecture is Option B followed by Option C:
first implement a minimal QPSK-like baseband prototype behind explicit real
prototype metadata, then expand toward OFDM/FEC/sync once frame/status
boundaries are stable.

Real 700F downselect remains prohibited.

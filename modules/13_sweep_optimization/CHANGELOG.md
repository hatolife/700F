# Changelog

## v0.2.0 - 2026-06-29
- Add ISSUE-0042 real-modem-prototype sweep row metadata for 700F-A minimal
  QPSK diagnostics while preserving 700F-B/C surrogate rows.

## v0.2.0 - 2026-06-28
- Mark 700F A/B/C campaign rows as ISSUE-0032 `surrogate_completed` rows with
  append-only not-real-modem/downselect/performance guardrails.
- Bridge M2 campaign mode registration to SSB, FreeDV emulator, and 700F
  candidate profile factories, with metadata-only completed rows for
  descriptor/profile-only modes and explicit official FreeDV skips.
- Add ISSUE-0026 M2 700F candidate smoke/full campaign helpers and TOML-friendly
  config shapes.
- Add ISSUE-0025 M2 smoke/full channel matrix config helpers.
- Reject duplicate channel condition ids and malformed channel numeric parameters in
  sweep orchestration.

## v0.1.0 - 2026-06-28
- Add initial sweep runner for stable mode/channel/seed expansion.
- Record skipped modes and failed runs without stopping later sweep records.
- Emit aggregate JSON/CSV summaries and document the M1 baseline smoke schema.

## v0.0.1 - 2026-06-27
- Bootstrap Sweep and Optimization Engine module skeleton.

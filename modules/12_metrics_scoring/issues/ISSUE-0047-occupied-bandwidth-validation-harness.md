# ISSUE-0047: occupied bandwidth validation harness

- Status: complete
- Priority: P0
- Owner: Module 12 + Module 15 + Module 07 + Module 13
- Branch: feature/ISSUE-0047-occupied-bandwidth-validation
- Target version: v0.3.0 target
- Last updated: 2026-06-29

## Scope

Module 12 owns the occupied bandwidth measurement utility and result/scoring
diagnostic fields for ISSUE-0047.

## Acceptance Criteria

- [x] Deterministic occupied bandwidth utility exists.
- [x] Empty and zero signals are safe.
- [x] Invalid sample rates are rejected.
- [x] Single-tone and two-tone estimates are covered by tests.
- [x] Result/scoring paths preserve occupied bandwidth estimate fields.
- [x] Limitations and 1.9 kHz target comparison are documented.

## TDD Record

- 2026-06-29: Added red contract tests for empty signal, zero signal, invalid
  sample rate, stable single-tone estimate, two-tone wider-than-single-tone
  estimate, and artifact field propagation before implementing the utility and
  serialization fields.

## Notes

The harness is a simple deterministic DFT/energy-ratio diagnostic. It is not a
regulatory occupied-bandwidth procedure and does not enable real 700F
downselect.

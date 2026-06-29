# ISSUE-0047: occupied bandwidth validation harness

- Status: complete
- Priority: P0
- Owner: Module 12 + Module 15 + Module 07 + Module 13
- Branch: feature/ISSUE-0047-occupied-bandwidth-validation
- Target version: v0.3.0 target
- Last updated: 2026-06-29

## Scope

Module 13 owns sweep-row propagation for occupied bandwidth diagnostics.

## Acceptance Criteria

- [x] Sweep JSON/CSV rows have append-only occupied bandwidth columns.
- [x] Completed real-modem-prototype rows can record the measurement from
  channel-output complex samples.
- [x] Downselect guardrails remain false/invalid for 700F prototype rows.

## TDD Record

- 2026-06-29: Reused focused sweep runner contract coverage after adding
  append-only occupied bandwidth row fields.

## Notes

The sweep runner does not make occupied bandwidth a ranking metric. It records
the value for Module 12 scoring/report diagnostics.

# ISSUE-0047: occupied bandwidth validation harness

- Status: complete
- Priority: P0
- Owner: Module 12 + Module 15 + Module 07 + Module 13
- Branch: feature/ISSUE-0047-occupied-bandwidth-validation
- Target version: v0.3.0 target
- Last updated: 2026-06-29

## Scope

Module 15 owns report-loader and Markdown visibility for occupied bandwidth
diagnostics produced by ISSUE-0047.

## Acceptance Criteria

- [x] Report loader accepts occupied bandwidth estimate fields from JSON/CSV
  sweep artifacts.
- [x] Markdown reports render estimate, target, delta, ratio, and status for
  real modem prototype diagnostics.
- [x] Report text preserves real downselect prohibition.

## TDD Record

- 2026-06-29: Added report contract coverage requiring the occupied bandwidth
  estimate and 1.9 kHz target comparison to appear in candidate summaries before
  updating renderer code.

## Notes

Occupied bandwidth values are displayed as limited prototype diagnostics only.
They do not affect final scoring or downselect feasibility.

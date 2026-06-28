# ISSUE-0025 M2 Expanded Channel Condition Matrix Handoff

## Summary

ISSUE-0025 defines the reusable M2 channel condition matrix on top of existing Module 08
channel primitives and Module 13 sweep expansion.

## Completed

- Documented smoke and full matrices in `docs/specs/channel_condition_matrix.md`.
- Added TOML-friendly examples:
  - `configs/channels/m2_channel_matrix_smoke.toml`
  - `configs/channels/m2_channel_matrix_full.toml`
- Added Module 13 helper configs for the smoke and full matrices.
- Added sweep-runner validation for duplicate condition ids, empty seed lists, and
  malformed numeric channel parameters.
- Added tests for matrix shape and rejection cases.

## Boundaries

- No new channel physics were introduced.
- No ISSUE-0019 through ISSUE-0022 work was redefined.
- No campaign mode selection, scoring policy, or report generation was added; those remain
  with ISSUE-0024, ISSUE-0026, and ISSUE-0027.

## Verification

- Targeted red test first: unresolved M2 helper references before implementation.
- Targeted `sweep_runner_contract`: passed after implementation.
- Full local CI: passed on 2026-06-28 with 10/10 CTest tests plus version and
  governance checks.

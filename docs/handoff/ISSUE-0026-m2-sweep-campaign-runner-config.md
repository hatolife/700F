# ISSUE-0026 M2 Sweep Campaign Runner Config Handoff

## Summary

ISSUE-0026 defines smoke and full M2 700F candidate sweep campaign configs by composing
the ISSUE-0025 channel matrices with the M2 baseline/candidate mode list.

## Completed

- Added Module 13 helpers:
  - `make_m2_700f_candidate_smoke_sweep_config()`
  - `make_m2_700f_candidate_full_sweep_config()`
- Added TOML-friendly campaign files:
  - `configs/sweeps/m2_700f_candidate_smoke.toml`
  - `configs/sweeps/m2_700f_candidate_full.toml`
- Added `docs/specs/m2_sweep_campaign.md` and updated sweep runner/API docs.
- Added tests for campaign mode ordering, smoke skip records, JSON/CSV aggregate
  emission, unknown/unavailable skip reasons, and stable repeated-run ordering.

## Boundaries

- No TOML parser was added.
- No new simulation/channel/scoring behavior was added.
- ISSUE-0019 through ISSUE-0022 were not redefined.
- With `F700F_ENABLE_CODEC2=OFF`, official FreeDV entries remain skip-capable.

## Verification

- Red test first: missing `make_m2_700f_candidate_smoke_sweep_config` helper.
- Targeted `sweep_runner_contract`: passed.
- Full local CI: passed on 2026-06-28 with 11/11 CTest tests plus version and
  governance checks.
- Build emitted clock-skew warnings on the shared filesystem; tests and checks passed.

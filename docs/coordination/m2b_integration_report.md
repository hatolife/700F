# M2-B Integration Report

## Summary

- Integration branch: `develop`
- Report time: 2026-06-28 22:30:51 +09:00
- Integrated issues: ISSUE-0024, ISSUE-0025, ISSUE-0026, ISSUE-0027
- Merge commits: ISSUE-0024 `bcad913`, ISSUE-0025 `8a2714e`, ISSUE-0026 `4ce1e3f`, ISSUE-0027 `088b7e0`
- Final local CI before manager report: passed with 12 CTest tests
- Push/PR: not performed

## Integrated Scope

- ISSUE-0024 added interim scoring policy and Module 12 scoring model.
- ISSUE-0025 added smoke/full M2 channel matrices and validation.
- ISSUE-0026 added M2 700F candidate smoke/full campaign configs and helper entry points.
- ISSUE-0027 added Module 15 baseline comparison Markdown rendering.

## Smoke Check

- Command: `bash -lc './build/test_sweep_runner && find build/test-artifacts/m2-700f-candidate-smoke -maxdepth 2 -type f | sort'`
- Artifacts:
  - `build/test-artifacts/m2-700f-candidate-smoke/m2-700f-candidate-smoke.json`
  - `build/test-artifacts/m2-700f-candidate-smoke/m2-700f-candidate-smoke.csv`
- Result: 30 skipped records, 0 completed, 0 failed.
- Common reason: `mode id not registered`.

## Comparison Report

- Timestamped report: `reports/20260628_223051_m2b_candidate_comparison.md`
- The report records that scoring/report components are integrated, but the smoke artifact contains only skipped rows.

## M2 Audit Gate

ISSUE-0023 can start only as a limited audit of the integrated M2-B plumbing. A meaningful candidate downselect should wait for ISSUE-0028 or an explicit decision to audit the skipped-only state.

## Follow-Up

- ISSUE-0028: connect M2 campaign modes to registered factories/profile-only factories.
- Existing known limitations remain: official FreeDV waveform round trip, SSB audio export, sweep CLI/TOML parser, report CLI/JSON loader, and clock-skew warnings.

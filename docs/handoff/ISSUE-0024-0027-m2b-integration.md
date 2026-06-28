# ISSUE-0024-0027 M2-B Integration Handoff

## Current State

- Current branch: `develop`
- Latest integrated M2-B merge before handoff commit: `088b7e0`
- Integrated issues: ISSUE-0024, ISSUE-0025, ISSUE-0026, ISSUE-0027
- Local CI: pass, 12 CTest tests
- Remote: `origin` is configured; push/PR not performed by instruction

## Smoke And Report

- M2-B smoke artifacts were generated under `build/test-artifacts/m2-700f-candidate-smoke/`.
- All 30 smoke rows were skipped because campaign modes are not registered in the smoke runner.
- M2-B candidate comparison report was recorded at `reports/20260628_223051_m2b_candidate_comparison.md`.

## Next Gate

- ISSUE-0028 should run before meaningful M2 downselect.
- ISSUE-0023 can start only as a limited audit unless the project accepts skipped-only evidence.

## Known Blockers

- M2 campaign mode factories are not registered.
- Official FreeDV waveform round trip remains incomplete.
- SSB audio export remains incomplete.
- Sweep CLI/TOML parser remains incomplete.
- Report generator has no CLI/JSON loader yet.
- Clock-skew warnings persist but do not block successful CI.

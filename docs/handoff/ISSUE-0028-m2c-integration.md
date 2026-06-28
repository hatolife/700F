# ISSUE-0028 M2-C Integration Handoff

## Current State

- Current branch after integration: `develop`
- Issue work branch: `feature/ISSUE-0028-m2-campaign-mode-registry`
- ISSUE-0028 commit: `a5020be`
- Local develop merge commit: `6d04f2a`
- Base commit before ISSUE-0028 commit: `7989d0c`
- Integrated issue: ISSUE-0028
- Local CI: pass, 12 CTest tests
- Remote: `origin` is configured; push/PR not performed by instruction

## Smoke And Report

- M2-C smoke artifacts were generated under
  `build/test-artifacts/m2-700f-candidate-smoke/`.
- Smoke result: 30 records, 21 completed, 0 failed, 9 skipped.
- Completed rows:
  - 6 SSB normal pipeline rows.
  - 6 FreeDV `descriptor_only_completed` rows.
  - 9 700F `profile_only_completed` rows.
- Skipped rows:
  - 6 official FreeDV rows with
    `official_waveform_roundtrip_not_implemented`.
  - 3 unknown-mode test probe rows.
- M2-C reports:
  - `reports/20260628_230402_m2c_smoke.md`
  - `reports/20260628_230402_m2c_candidate_comparison.md`
  - `reports/20260628_230402_m2c_integration.md`
  - `docs/coordination/m2c_integration_report.md`

## Next Gate

- ISSUE-0023 can start as a limited audit of M2-C readiness evidence.
- Real candidate downselect remains blocked until waveform-capable 700F rows and
  official baseline roundtrip rows exist.

## Known Blockers

- Official FreeDV waveform round trip remains incomplete.
- 700F candidates are profile-only and do not provide waveform performance.
- FreeDV emulator rows are descriptor-only in this bridge.
- SSB audio export remains incomplete.
- Sweep CLI/TOML parser remains incomplete.
- Report generator has no CLI/JSON loader yet.
- Clock-skew warnings persist but do not block successful CI.

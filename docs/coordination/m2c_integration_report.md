# M2-C Integration Report

## Summary

- Branch: `feature/ISSUE-0028-m2-campaign-mode-registry`
- Merge target: `develop`
- Issue: ISSUE-0028
- Report time: 2026-06-28 23:04:02 +09:00
- Local CI: passed with 12 CTest tests
- Remote: `origin` configured; push/PR not performed by instruction

## Outcome

M2-C bridges the campaign smoke runner to registered SSB reference, FreeDV
emulator, and 700F candidate profile factories. Smoke artifacts are no longer
all skipped.

## Smoke Counts

| Status | Count |
|---|---:|
| completed | 21 |
| failed | 0 |
| skipped | 9 |

| Row type | Count |
|---|---:|
| SSB normal pipeline | 6 |
| FreeDV descriptor-only | 6 |
| 700F profile-only | 9 |
| Official FreeDV explicit skips | 6 |
| Unknown-mode test skips | 3 |

## Audit Gate

ISSUE-0023 is startable as a limited audit. It should not perform or imply a
real candidate downselect until waveform-capable 700F and official baseline
roundtrip rows exist.

## Reports

- `reports/20260628_230402_m2c_smoke.md`
- `reports/20260628_230402_m2c_candidate_comparison.md`
- `reports/20260628_230402_m2c_integration.md`
- `docs/handoff/ISSUE-0028-m2c-integration.md`

# M2-D Partial Integration Report

## Status

ISSUE-0029, ISSUE-0030, and ISSUE-0031 are integrated locally into `develop`.

## Verification

- `bash ./tools/run_ci_local.sh`: passed.
- CTest: 20/20 passed.
- `f700f-sweep` CLI smoke: passed.
- `f700f-report` CLI comparison report: passed.

## Smoke Counts

| Status | Count |
|---|---:|
| completed | 21 |
| failed | 0 |
| skipped | 6 |

| Row type | Count |
|---|---:|
| SSB normal pipeline | 6 |
| FreeDV descriptor-only | 6 |
| 700F profile-only | 9 |
| Official FreeDV explicit skips | 6 |

## Decision Boundary

Real candidate downselect remains prohibited. The current M2-D result validates
tooling and artifact loading, not RF/audio performance.

## Reports

- `reports/20260629_003746_m2d_smoke.md`
- `reports/20260629_003746_m2d_candidate_comparison.md`
- `reports/20260629_003746_m2d_partial_integration.md`
- `docs/handoff/ISSUE-0023-0031-m2d-entry.md`

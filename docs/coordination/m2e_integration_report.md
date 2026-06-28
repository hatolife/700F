# M2-E Integration Report

## Summary

M2-E integrated ISSUE-0032 through ISSUE-0035 into local `develop` and produced a
fresh M2-E smoke sweep. The project can now compare clearly labeled SSB,
700D/700E emulated-surrogate, 700F surrogate, and official-unavailable rows in the
same report without implying a real 700F downselect.

## Integrated Issues

| Issue | Status | Commit | Result |
|---|---|---|---|
| ISSUE-0032 | complete-local | `ce12a08` | 700F A/B/C complete as synthetic surrogate rows. |
| ISSUE-0033 | complete-local | `20a4005` | 700D/700E emulators complete as non-official emulated-surrogate rows. |
| ISSUE-0034 | complete-local | `b0d74e5` | Official FreeDV source research and guarded blockers recorded. |
| ISSUE-0035 | complete-local | `9e3119e` | Clock-skew warning handling documented as non-blocking when CI passes. |

## Smoke Result

- Command: `./build/f700f-sweep --config configs/sweeps/m2_700f_candidate_smoke.toml --output-dir artifacts/sweeps/m2e_smoke --run-id m2e-smoke`
- Result: completed 21, skipped 6, failed 0
- Total rows: 27
- 700F surrogate rows: 9
- 700D/700E emulator surrogate rows: 6
- Official FreeDV skipped rows: 6
- Report: `reports/20260629_024549_m2e_candidate_comparison.md`

## Decision

M2-E is complete with limitations. Real downselect remains blocked until ISSUE-0036
or successor work provides official FreeDV runtime rows and later 700F work provides
waveform-capable performance rows.

## Publication

Push/PR was not performed by request.

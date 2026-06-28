# M2-B Candidate Comparison Report

## Summary

- Branch: `develop`
- Commit: `088b7e0`
- Report time: 2026-06-28 22:30:51 +09:00
- Config: `configs/sweeps/m2_700f_candidate_smoke.toml`
- Sweep status: generated artifacts with all rows skipped
- Report generator status: Module 15 deterministic Markdown renderer is integrated, but no CLI/JSON loader exists yet.

## Score Output

No meaningful score table was produced from the smoke artifacts because every campaign row was skipped before simulation. The scoring model is integrated and tested independently by `scoring_model_contract`.

## Mode Status

| Mode | Status |
|---|---|
| `ssb_standard_3k` | skipped: mode id not registered |
| `ssb_narrow_1k9` | skipped: mode id not registered |
| `freedv700d_emulated` | skipped: mode id not registered |
| `freedv700e_emulated` | skipped: mode id not registered |
| `freedv700d_official` | skipped: mode id not registered |
| `freedv700e_official` | skipped: mode id not registered |
| `freedv700f_a_balanced` | skipped: mode id not registered |
| `freedv700f_b_robust` | skipped: mode id not registered |
| `freedv700f_c_quality` | skipped: mode id not registered |

## Channel Conditions

- `identity`
- `awgn-snr-6db`
- `awgn-snr-0db`

## Known Limitations

- Mode factories are not registered in the M2 smoke runner.
- Official FreeDV waveform round trip remains incomplete.
- SSB audio export remains incomplete.
- Sweep CLI/TOML parser remains incomplete.
- Report generator has no CLI/JSON loader yet.

## Recommendation

Start ISSUE-0028 before ISSUE-0023 candidate downselect. ISSUE-0023 can audit the M2-B path if desired, but downselect evidence is not useful until at least one campaign mode produces a non-skipped result.

# M2-C Candidate Comparison Report

## Summary

- Branch: `feature/ISSUE-0028-m2-campaign-mode-registry`
- Base commit before ISSUE-0028 commit: `7989d0c`
- Report time: 2026-06-28 23:04:02 +09:00
- Config basis: `configs/sweeps/m2_700f_candidate_smoke.toml`
- Sweep status: generated artifacts with 21 completed rows and 9 skipped rows.
- Report generator status: Module 15 deterministic Markdown renderer is integrated,
  but no CLI/JSON loader exists yet.

## Score Output

No real candidate downselect score is reported. The completed FreeDV emulator and
700F candidate rows are descriptor/profile-only readiness records, not waveform
performance measurements. SSB reference rows execute through the placeholder
reference path and can be used only as plumbing evidence at this stage.

## Mode Status

| Mode | Status |
|---|---|
| `ssb_standard_3k` | completed through normal pipeline |
| `ssb_narrow_1k9` | completed through normal pipeline |
| `freedv700d_emulated` | completed: `descriptor_only_completed` |
| `freedv700e_emulated` | completed: `descriptor_only_completed` |
| `freedv700d_official` | skipped: `official_waveform_roundtrip_not_implemented` |
| `freedv700e_official` | skipped: `official_waveform_roundtrip_not_implemented` |
| `freedv700f_a_balanced` | completed: `profile_only_completed` |
| `freedv700f_b_robust` | completed: `profile_only_completed` |
| `freedv700f_c_quality` | completed: `profile_only_completed` |

## Channel Conditions

- `identity`
- `awgn-snr-6db`
- `awgn-snr-0db`

## Known Limitations

- Official FreeDV waveform round trip remains incomplete.
- 700F candidates are profile-only and do not emit real waveform performance.
- FreeDV emulator rows are descriptor-only in this bridge.
- SSB audio export remains incomplete.
- Sweep CLI and TOML parsing remain incomplete.
- Report generator has no CLI/JSON loader yet.

## Recommendation

Start ISSUE-0023 only as a limited audit of M2-C readiness evidence. Do not make a
real candidate downselect until waveform-capable 700F rows and official baseline
roundtrip rows exist.

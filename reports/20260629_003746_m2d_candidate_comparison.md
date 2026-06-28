# M2 Baseline Comparison Report

- Run id: `m2d-smoke`
- Commit: `unknown`
- Config: `unknown`
- Generated at UTC: `unknown`
- Score policy: `m2-interim-v0`
- Total result records: 27
- Sweep status: loaded from artifact

- Downselect feasibility: Real downselect possible: no; skipped, profile-only, descriptor-only, or failed rows prevent real downselect.

## Mode List

`freedv700d_emulated`, `freedv700d_official`, `freedv700e_emulated`, `freedv700e_official`, `freedv700f_a_balanced`, `freedv700f_b_robust`, `freedv700f_c_quality`, `ssb_narrow_1k9`, `ssb_standard_3k`

## Channel List

`awgn-snr-0db`, `awgn-snr-6db`, `identity`

## Seeds

`1`

## Score Table

| Mode | Score | Completed | Failed | Skipped | Official unavailable | Profile-only | Status | Notes |
|---|---:|---:|---:|---:|---:|---:|---|---|
| `freedv700d_emulated` | 100 | 3 | 0 | 0 | 0 | 0 | descriptor_only |  |
| `freedv700e_emulated` | 100 | 3 | 0 | 0 | 0 | 0 | descriptor_only |  |
| `freedv700f_a_balanced` | 100 | 3 | 0 | 0 | 0 | 3 | profile_only | profile_only |
| `freedv700f_b_robust` | 100 | 3 | 0 | 0 | 0 | 3 | profile_only | profile_only |
| `freedv700f_c_quality` | 100 | 3 | 0 | 0 | 0 | 3 | profile_only | profile_only |
| `ssb_narrow_1k9` | 100 | 3 | 0 | 0 | 0 | 0 | emulated |  |
| `ssb_standard_3k` | 91.316 | 3 | 0 | 0 | 0 | 0 | emulated |  |
| `freedv700d_official` | 0 | 0 | 0 | 3 | 3 | 0 | official | official adapter unavailable |
| `freedv700e_official` | 0 | 0 | 0 | 3 | 3 | 0 | official | official adapter unavailable |

## Skipped And Unavailable Modes

- `freedv700d_official` skipped 3, official unavailable 3
- `freedv700e_official` skipped 3, official unavailable 3

## Raw Metrics Summary

| Mode | Records | mean dropout | mean latency s | max RF bandwidth Hz | BER available | FER available | BER unavailable | FER unavailable | ASR WER slots | STOI slots | ESTOI slots | Subjective note slots |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `freedv700d_emulated` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `freedv700e_emulated` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `freedv700f_a_balanced` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `freedv700f_b_robust` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `freedv700f_c_quality` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `ssb_narrow_1k9` | 3 | 0 | 0 | 1900 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `ssb_standard_3k` | 3 | 0 | 0 | 3000 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `freedv700d_official` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `freedv700e_official` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |

## Baseline And Candidate Status

- Official FreeDV status: unavailable/skipped: 6
- SSB reference status: completed records: 6
- 700F candidate summary:
- `freedv700f_a_balanced` profile_only score 100
- `freedv700f_b_robust` profile_only score 100
- `freedv700f_c_quality` profile_only score 100

## Known Limitations

- Real downselect possible: no; skipped, profile-only, descriptor-only, or failed rows prevent real downselect.
- Profile-only rows are non-performance evidence.
- Descriptor-only rows are non-performance evidence.
- Skipped row `freedv700d_official`: official_waveform_roundtrip_not_implemented: default M2 smoke keeps official FreeDV skipped until Codec2 official roundtrip is wired
- Skipped row `freedv700d_official`: official_waveform_roundtrip_not_implemented: default M2 smoke keeps official FreeDV skipped until Codec2 official roundtrip is wired
- Skipped row `freedv700d_official`: official_waveform_roundtrip_not_implemented: default M2 smoke keeps official FreeDV skipped until Codec2 official roundtrip is wired
- Skipped row `freedv700e_official`: official_waveform_roundtrip_not_implemented: default M2 smoke keeps official FreeDV skipped until Codec2 official roundtrip is wired
- Skipped row `freedv700e_official`: official_waveform_roundtrip_not_implemented: default M2 smoke keeps official FreeDV skipped until Codec2 official roundtrip is wired
- Skipped row `freedv700e_official`: official_waveform_roundtrip_not_implemented: default M2 smoke keeps official FreeDV skipped until Codec2 official roundtrip is wired

## Next recommendation placeholder

Pending M2 downselect review after complete sweep evidence.

# M2 Baseline Comparison Report

- Run id: `m2h-700f-qpsk-smoke-codec2-on`
- Commit: `unknown`
- Config: `unknown`
- Generated at UTC: `unknown`
- Score policy: `m2-interim-v0`
- Total result records: 27
- Sweep status: loaded from artifact

- Downselect feasibility: Real downselect possible: no; surrogate, waveform-prototype, skipped, real-modem-prototype, profile-only, descriptor-only, emulated-surrogate, or failed rows prevent real downselect.

## Mode List

`freedv700d_emulated`, `freedv700d_official`, `freedv700e_emulated`, `freedv700e_official`, `freedv700f_a_balanced`, `freedv700f_b_robust`, `freedv700f_c_quality`, `ssb_narrow_1k9`, `ssb_standard_3k`

## Channel List

`awgn-snr-0db`, `awgn-snr-6db`, `identity`

## Seeds

`1`

## Score Table

| Mode | Score | Completed | Failed | Skipped | Official unavailable | Profile-only | 700F surrogate | Emulated surrogate | Performance valid | Performance invalid | Real score | Surrogate readiness | Status | Notes |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---|
| `freedv700d_official` | 100 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 100 | 0 | official |  |
| `freedv700e_official` | 100 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 100 | 0 | official |  |
| `ssb_narrow_1k9` | 100 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 100 | 0 | emulated |  |
| `ssb_standard_3k` | 91.316 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 91.316 | 0 | emulated |  |
| `freedv700d_emulated` | 0 | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 3 | 0 | 0 | emulated_surrogate | performance invalid; not_official_freedv=true |
| `freedv700e_emulated` | 0 | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 3 | 0 | 0 | emulated_surrogate | performance invalid; not_official_freedv=true |
| `freedv700f_a_balanced` | 0 | 3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 | 0 | real_modem_prototype | REAL MODEM PROTOTYPE WARNING: performance_valid=limited performance_validity=limited downselect_valid=false downselect_validity=invalid |
| `freedv700f_b_robust` | 0 | 3 | 0 | 0 | 0 | 0 | 3 | 0 | 0 | 3 | 0 | 62.5 | surrogate | SURROGATE WARNING: not_real_modem=true downselect_valid=false performance_valid=false |
| `freedv700f_c_quality` | 0 | 3 | 0 | 0 | 0 | 0 | 3 | 0 | 0 | 3 | 0 | 62.5 | surrogate | SURROGATE WARNING: not_real_modem=true downselect_valid=false performance_valid=false |

## Skipped And Unavailable Modes

- No skipped or unavailable modes were scored.

## Raw Metrics Summary

| Mode | Records | mean dropout | mean latency s | max RF bandwidth Hz | BER available | FER available | BER unavailable | FER unavailable | ASR WER slots | STOI slots | ESTOI slots | Subjective note slots |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `freedv700d_official` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `freedv700e_official` | 3 | 0 | 0 | 1600 | 0 | 0 | 3 | 3 | 0 | 0 | 0 | 0 |
| `ssb_narrow_1k9` | 3 | 0 | 0 | 1900 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `ssb_standard_3k` | 3 | 0 | 0 | 3000 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `freedv700d_emulated` | 3 | 0 | 0 | 1600 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `freedv700e_emulated` | 3 | 0 | 0 | 1600 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `freedv700f_a_balanced` | 3 | 0 | 0 | 1600 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `freedv700f_b_robust` | 3 | 0 | 0 | 1600 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `freedv700f_c_quality` | 3 | 0 | 0 | 1600 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

## Baseline And Candidate Status

- Official FreeDV status: unavailable/skipped: 0
- SSB reference status: completed records: 6
- 700F candidate summary:
- `freedv700f_a_balanced` real_modem_prototype real score 0 surrogate readiness 0 REAL MODEM PROTOTYPE WARNING prototype=true not_final_modem=true downselect_valid=false downselect_validity=invalid performance_valid=limited performance_validity=limited sync_family=none modem_family=minimal_qpsk prototype_frame_status=completed prototype_sync_status=none
- `freedv700f_b_robust` surrogate real score 0 surrogate readiness 62.5 not_real_modem=true downselect_valid=false performance_valid=false synthetic_surrogate_readiness_only model=700f_candidate_minimal_behavior
- `freedv700f_c_quality` surrogate real score 0 surrogate readiness 62.5 not_real_modem=true downselect_valid=false performance_valid=false synthetic_surrogate_readiness_only model=700f_candidate_minimal_behavior

## Known Limitations

- Real downselect possible: no; surrogate, waveform-prototype, skipped, real-modem-prototype, profile-only, descriptor-only, emulated-surrogate, or failed rows prevent real downselect.
- SURROGATE WARNING: 700F surrogate rows are synthetic readiness evidence only; not_real_modem=true; downselect_valid=false; performance_valid=false.
- REAL MODEM PROTOTYPE WARNING: prototype diagnostics are limited engineering evidence only; performance_validity=limited; downselect_valid=false; downselect_validity=invalid.
- Emulated surrogate rows are non-official and non-performance evidence: implementation_status=emulated_surrogate; not_official_freedv=true; downselect_valid=false; performance_valid=false; emulator_limitations=deterministic surrogate only; not official FreeDV performance

## Next recommendation placeholder

Pending M2 downselect review after complete sweep evidence.

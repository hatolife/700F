# 700F Candidate Profiles v0.2.0

ISSUE-0018 introduced three experimental 700F candidate profiles. ISSUE-0032 made
them selectable surrogate Mode descriptors for sweep/report readiness. ISSUE-0039
upgrades only 700F-A to a minimal waveform-capable prototype. They are not final
modem implementations.

## Profile Table

| Mode id | Goal | Occupied bandwidth target | Audio bandwidth target | Robustness | FEC policy | Latency | Intended channel |
|---|---|---:|---:|---|---|---|---|
| `freedv700f_a_balanced` | SSB average intelligibility exceedance | 1.9 kHz | 3.0 kHz | medium | medium | medium | average HF QSO |
| `freedv700f_b_robust` | fading/SNR robustness | 1.9 kHz | 2.7-3.0 kHz | high | stronger | medium-high | weak/unstable HF |
| `freedv700f_c_quality` | perceived quality under good channel | 1.9 kHz | 3.3-4.0 kHz | medium-low | lighter | medium | good HF path |

## Descriptor Mapping

All profiles set:

- `official_baseline = false`
- `emulator = false`
- `rf_bandwidth_hz = 1900.0`
- audio, complex, bit-payload, and soft-bit capabilities enabled
- codec, FEC, modem, modulation, and pilot ids to candidate or prototype values

Implementation status by profile:

- `freedv700f_a_balanced`: `implementation_status = "waveform_prototype"`,
  `codec_id = "synthetic-700f-a-prototype"`, `fec_id = "none"`,
  `modem_id = "toy_audio_waveform-700f-a"`, and
  `modulation_family = "toy_audio_waveform"`.
- `freedv700f_b_robust`: `implementation_status = "surrogate"`.
- `freedv700f_c_quality`: `implementation_status = "surrogate"`.

Specific descriptor targets:

| Mode id | Audio low | Audio high | Audio bandwidth | FEC id |
|---|---:|---:|---:|---|
| `freedv700f_a_balanced` | 300 Hz | 3300 Hz | 3000 Hz | `none` |
| `freedv700f_b_robust` | 300 Hz | 3150 Hz | 2850 Hz | `700f-candidate-strong-fec-policy` |
| `freedv700f_c_quality` | 300 Hz | 4000 Hz | 3700 Hz | `700f-candidate-light-fec-policy` |

The quality profile uses a 12 kHz descriptor sample rate so its upper audio target stays
inside the sampled passband.

## Runtime Behavior

Profile factories are available through:

- `make_700f_candidate_profile_factory(mode_id)`
- `register_700f_candidate_profiles(registry)`

The runtime accepts default configuration or the descriptor sample rate.
`freedv700f_a_balanced` performs deterministic toy audio-to-complex encode and
complex-to-audio decode so the existing pipeline can exercise a waveform-capable
prototype path. `freedv700f_b_robust` and `freedv700f_c_quality` return
`ok == false` and an error containing the mode id, `ISSUE-0032`, `surrogate`,
and `not_real_modem`.

## Reporting Expectations

Metrics snapshots, simulation reports, and sweep reports must preserve the canonical
mode id. ISSUE-0039 700F-A rows complete through the pipeline with
`prototype = true`, `not_final_modem = true`, `waveform_capable = true`,
`downselect_valid = false`, and `performance_valid = false`. ISSUE-0032 B/C
candidate sweep records complete through the surrogate bridge with
`not_real_modem = true`, `downselect_valid = false`, and
`performance_valid = false`. BER/FER are not emitted as real 700F performance
values, and any synthetic readiness metrics must be labeled synthetic. Missing
candidate factories may still be recorded as skipped by sweep infrastructure.

# 700F Candidate Profiles v0.2.0

ISSUE-0018 introduces three experimental 700F candidate profiles. They are selectable
Mode descriptors and profile-only runtime stubs, not modem implementations.

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
- `implementation_status = "profile_only"`
- `rf_bandwidth_hz = 1900.0`
- audio, complex, bit-payload, and soft-bit capabilities enabled
- codec, FEC, modem, modulation, and pilot ids to candidate placeholder values

Specific descriptor targets:

| Mode id | Audio low | Audio high | Audio bandwidth | FEC id |
|---|---:|---:|---:|---|
| `freedv700f_a_balanced` | 300 Hz | 3300 Hz | 3000 Hz | `700f-candidate-medium-fec-policy` |
| `freedv700f_b_robust` | 300 Hz | 3150 Hz | 2850 Hz | `700f-candidate-strong-fec-policy` |
| `freedv700f_c_quality` | 300 Hz | 4000 Hz | 3700 Hz | `700f-candidate-light-fec-policy` |

The quality profile uses a 12 kHz descriptor sample rate so its upper audio target stays
inside the sampled passband.

## Runtime Behavior

Profile factories are available through:

- `make_700f_candidate_profile_factory(mode_id)`
- `register_700f_candidate_profiles(registry)`

The runtime accepts default configuration or the descriptor sample rate. Encode and
decode return `ok == false` and an error containing the mode id, `ISSUE-0018`, and
`profile_only`.

## Reporting Expectations

Metrics snapshots, simulation reports, and sweep reports must preserve the canonical
mode id. Profile-only candidate sweep records are expected to fail at encode/decode
rather than skip once their factory is registered. Missing candidate factories may still
be recorded as skipped by sweep infrastructure.

# FreeDV 700D/700E Parameterized Emulator Skeleton

ISSUE-0013 adds emulator-only Mode interface skeletons for `freedv700d_emulated` and
`freedv700e_emulated`. These modes are not official Codec2/FreeDV baselines and must not
be used as validation references.

## Mode identity

Both descriptors set `emulator = true` and `official_baseline = false`. The official
baseline adapter remains separate under ISSUE-0012 and is intentionally not implemented
or edited by this work.

## Initial descriptor assumptions

The values below are placeholders chosen to exercise the Mode interface and downstream
registry plumbing. They are documented assumptions, not authoritative FreeDV values.

| Mode id | Display name | RF bandwidth | Audio passband | Frame duration | CP duration | Raw bitrate | Carriers | Modulation family | FEC name | Pilot strategy |
|---|---|---:|---:|---:|---:|---:|---:|---|---|---|
| `freedv700d_emulated` | FreeDV 700D parameterized emulator | 1500 Hz | 300-3000 Hz | 40 ms | 2.5 ms | 700 bps | 14 | `dqpsk-ofdm-placeholder` | `parameterized-freedv-fec-placeholder` | `scattered-pilot-placeholder` |
| `freedv700e_emulated` | FreeDV 700E parameterized emulator | 1600 Hz | 300-3000 Hz | 40 ms | 2.5 ms | 700 bps | 17 | `dqpsk-ofdm-placeholder` | `parameterized-freedv-fec-placeholder` | `700e-style scattered-pilot-placeholder` |

## Runtime behavior

`configure` accepts the descriptor sample rate or sample rate `0` for default selection.
`encode` and `decode` currently return `ok = false` with an ISSUE-0013 "not implemented"
message. No dummy waveform, Codec2 adapter, FEC, interleaver, synchronizer, or official
FreeDV modem behavior is implemented.

## Replacement boundary

The skeleton is intentionally descriptor-first. Later work can replace the runtime with a
higher-quality emulator while preserving the Mode ids and descriptor fields. Any official
Codec2/FreeDV comparison belongs to the ISSUE-0012 official baseline adapter, not this
emulator skeleton.

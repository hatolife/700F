# FreeDV Emulator Waveform Assumptions

ISSUE-0013 records the OFDM-related descriptor assumptions used by the parameterized
emulator skeletons. These are not official FreeDV values.

| Mode id | Carriers | CP duration | Modulation family | Pilot strategy |
|---|---:|---:|---|---|
| `freedv700d_emulated` | 14 | 2.5 ms | `dqpsk-ofdm-placeholder` | `scattered-pilot-placeholder` |
| `freedv700e_emulated` | 17 | 2.5 ms | `dqpsk-ofdm-placeholder` | `700e-style scattered-pilot-placeholder` |

The Module 07 contribution is descriptor metadata only. High-fidelity OFDM modulation,
carrier mapping, pilot insertion, synchronization, and channel-aware demodulation remain
future work.

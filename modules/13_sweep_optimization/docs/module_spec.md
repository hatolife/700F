# Module Spec

Module 13 owns sweep orchestration for baseline and M2 candidate-supporting simulation.

The v0.1.0 runner expands a structured config across modes, channel
conditions, and seeds, then delegates each attempted run to Module 11's
deterministic simulation pipeline. It records skipped modes and failed runs as
data, writes aggregate JSON/CSV summaries, and preserves input ordering for
reproducible short-CI smoke sweeps.

ISSUE-0025 adds reusable M2 channel matrix helpers and validation while preserving this
expansion model. The smoke matrix contains identity, AWGN 6 dB, AWGN 0 dB, and seed 1.
The full matrix contains 72 channel conditions from SNR, frequency-offset, and fading
axes plus seeds 1, 2, and 3.

ISSUE-0051 adds a short M3 channel impairment smoke campaign with four
conditions: identity baseline, AWGN 3 dB, 75 Hz frequency offset, and a combined
AWGN 6 dB plus 50 Hz frequency offset plus weak simple-gain fading chain. It
uses deterministic seed 70051 and the existing nine-mode campaign ordering.

Optimization search, TOML parsing, SSB fidelity, 700F waveform implementation, and
official Codec2/FreeDV adapters remain out of scope here.

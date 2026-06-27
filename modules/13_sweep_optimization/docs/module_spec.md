# Module Spec

Module 13 owns sweep orchestration for M1 baseline simulation.

The v0.1.0 runner expands a structured config across modes, channel
conditions, and seeds, then delegates each attempted run to Module 11's
deterministic simulation pipeline. It records skipped modes and failed runs as
data, writes aggregate JSON/CSV summaries, and preserves input ordering for
reproducible short-CI smoke sweeps.

Optimization search, TOML parsing, SSB fidelity, 700F candidates, and official
Codec2/FreeDV adapters are out of scope for ISSUE-0015.

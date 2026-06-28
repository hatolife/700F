# Limitations

ISSUE-0015 v0.1.0 is a short-CI sweep runner, not an optimization engine.

- ISSUE-0030 implements a minimal TOML-shaped loader for the current
  `configs/sweeps/*.toml` schema. Richer TOML schema work, including shared
  channel includes and file-audio inputs, remains planned.
- Missing or unmerged modes are recorded as skipped sweep records.
- Official FreeDV 700D/700E modes are expected to skip when
  `F700F_ENABLE_CODEC2=OFF` until the optional Codec2 adapter is implemented.
- SSB and FreeDV waveform fidelity belongs to their reference-mode issues, not
  this module.

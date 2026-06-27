# Limitations

ISSUE-0015 v0.1.0 is a short-CI sweep runner, not an optimization engine.

- TOML parsing is deferred; `configs/sweeps/m1_baseline_smoke.toml` is a schema
  example and the implemented entry point is `f700f::SweepConfig`.
- Missing or unmerged modes are recorded as skipped sweep records.
- Official FreeDV 700D/700E modes are expected to skip when
  `F700F_ENABLE_CODEC2=OFF` until the optional Codec2 adapter is implemented.
- SSB and FreeDV waveform fidelity belongs to their reference-mode issues, not
  this module.

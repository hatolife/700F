# Api Contract

Module 13 exposes the C++ sweep surface in `include/f700f/sweep_runner.hpp`.

- `SweepConfig` expands as `modes x channel_conditions x seeds`.
- `make_m1_baseline_smoke_sweep_config()` returns the M1 baseline smoke config.
- `make_m2_channel_matrix_smoke_sweep_config()` returns the ISSUE-0025 smoke channel
  matrix config fragment.
- `make_m2_channel_matrix_full_sweep_config()` returns the ISSUE-0025 full channel
  matrix config fragment.
- `make_m2_700f_candidate_smoke_sweep_config()` returns the ISSUE-0026 smoke campaign
  config with M2 baseline/candidate mode ordering.
- `make_m2_700f_candidate_full_sweep_config()` returns the ISSUE-0026 full campaign
  config with the same mode ordering over the full M2 channel matrix.

Campaign helpers keep the existing `modes x channel_conditions x seeds` expansion and
do not add TOML parsing.

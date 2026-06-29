# Api Contract

Module 13 exposes the C++ sweep surface in `include/f700f/sweep_runner.hpp`.

- `SweepConfig` expands as `modes x channel_conditions x seeds`.
- Each completed/failed sweep record records `audio_export_path`, which is the
  simulated decoded-audio placeholder path when export is enabled and `N/A`
  otherwise.
- `make_m1_baseline_smoke_sweep_config()` returns the M1 baseline smoke config.
- `make_m2_channel_matrix_smoke_sweep_config()` returns the ISSUE-0025 smoke channel
  matrix config fragment.
- `make_m2_channel_matrix_full_sweep_config()` returns the ISSUE-0025 full channel
  matrix config fragment.
- `make_m2_700f_candidate_smoke_sweep_config()` returns the ISSUE-0026 smoke campaign
  config with M2 baseline/candidate mode ordering.
- `make_m2_700f_candidate_full_sweep_config()` returns the ISSUE-0026 full campaign
  config with the same mode ordering over the full M2 channel matrix.
- `make_m3_channel_impairment_smoke_sweep_config()` returns the ISSUE-0051 short
  M3 impairment regression campaign over identity, AWGN, frequency offset, and a
  combined weak-fading chain with deterministic seed `70051`.
- `register_m2_campaign_mode_factories()` registers default M2 smoke-capable mode
  factories: SSB references, descriptor-only FreeDV emulators, and 700F
  surrogate candidate factories. Official FreeDV modes are intentionally not
  registered in default Codec2-disabled smoke runs and remain explicit skips.
- `load_sweep_config_from_file()` loads the supported sweep TOML subset into
  `SweepConfig`; `SweepConfigOverrides` can replace `output_directory` and
  `run_id_prefix` after parsing.
- `f700f-sweep` is the user-facing CLI for running supported sweep config files
  outside the test binary.

Campaign helpers keep the existing `modes x channel_conditions x seeds` expansion and
the CLI uses the same runner/artifact contract.

ISSUE-0032 appends surrogate guardrail fields to each `SweepRunRecord` and aggregate
JSON/CSV row. 700F-B/C candidate rows complete as `surrogate_completed` with
`implementation_status = surrogate`, `not_real_modem = true`,
`downselect_valid = false`, `not_downselect_valid = true`, and
`performance_valid = false`; any readiness value is labeled
`surrogate_readiness_score_synthetic`.

ISSUE-0042 appends real-prototype guardrail and diagnostic fields for 700F-A:
`implementation_classification`, `performance_validity`,
`downselect_validity`, `sync_family`, `prototype_warning`,
`prototype_symbol_error_rate`, `prototype_frame_status`,
`prototype_sync_status`, and `prototype_baseband_sample_count`. 700F-A rows
complete as `real_modem_prototype_completed` with
`implementation_status = real_modem_prototype`, `modem_family = minimal_qpsk`,
and `downselect_valid = false`.

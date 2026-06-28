# M2 Sweep Campaign v0.2.0

ISSUE-0026 defines the M2 700F candidate sweep campaign configs that compose the
ISSUE-0025 channel matrices with the M2 baseline/candidate mode list.

The runner expansion contract remains:

`modes x channel_conditions x seeds`

## Campaign Modes

The campaign mode order is stable and is used as the first expansion axis:

1. `ssb_standard_3k`
2. `ssb_narrow_1k9`
3. `freedv700d_emulated`
4. `freedv700e_emulated`
5. `freedv700d_official`
6. `freedv700e_official`
7. `freedv700f_a_balanced`
8. `freedv700f_b_robust`
9. `freedv700f_c_quality`

Every entry is skip-capable. Unknown modes, unregistered official FreeDV adapters
under `F700F_ENABLE_CODEC2=OFF`, and candidates that are not registered by a caller
are recorded as skipped by the sweep runner.

If an ISSUE-0032 700F candidate factory is registered, the sweep row completes through
the surrogate bridge with `implementation_status = surrogate`, `not_real_modem = true`,
`downselect_valid = false`, and `performance_valid = false`. That completed surrogate
evidence must not be interpreted as real performance evidence.

## Smoke Campaign

`make_m2_700f_candidate_smoke_sweep_config()` returns:

- run id prefix `m2-700f-candidate-smoke`;
- generated 8 kHz, 32-sample tone input;
- metric ids `dummy.metric`;
- the smoke channel matrix from ISSUE-0025: `identity`, `awgn-snr-6db`,
  `awgn-snr-0db`;
- seed `1`;
- the nine campaign modes above.

The TOML-friendly shape is recorded in
`configs/sweeps/m2_700f_candidate_smoke.toml`.

## Full Campaign

`make_m2_700f_candidate_full_sweep_config()` returns:

- run id prefix `m2-700f-candidate-full`;
- generated 8 kHz, 32-sample tone input;
- metric ids `dummy.metric`;
- the full ISSUE-0025 matrix: 72 channel conditions;
- seeds `1`, `2`, and `3`;
- the nine campaign modes above.

This yields 1,944 planned records before registration/runtime availability is applied:
`9 modes x 72 channel conditions x 3 seeds`.

The TOML-friendly axis shape is recorded in
`configs/sweeps/m2_700f_candidate_full.toml`.

## Artifacts And Ordering

When `output_directory` is set, both campaigns emit aggregate JSON and CSV files using
the existing sweep artifact contract. Per-run simulation summaries are emitted only for
attempted runs through the existing Module 11 runner path.

Result ordering is deterministic for the same config and seed order:

1. mode order from the campaign config;
2. channel condition order from the selected matrix;
3. seed order from the campaign config.

The smoke test verifies JSON/CSV aggregate emission, unknown/unavailable mode skip
reasons, and stable repeated-run ordering.

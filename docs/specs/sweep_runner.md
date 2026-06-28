# Sweep Runner v0.1.0

ISSUE-0015 adds a minimal short-CI sweep runner for deterministic baseline
simulation. It expands a structured sweep config into stable single-run
`SimulationConfig` records:

`modes x channel_conditions x seeds`

The runner is orchestration only. It does not implement 700F optimization, SSB
emulation, or the official Codec2/FreeDV adapter.

## Configuration

The v0.1.0 C++ config lives in `f700f::SweepConfig`:

- `run_id_prefix`: prefix for aggregate artifacts and per-run ids.
- `output_directory`: directory for aggregate JSON/CSV and per-run summaries.
- `input`: forwarded to the simulation pipeline; generated tones are supported.
- `metric_ids`: ordered metrics forwarded to each single run.
- `modes`: ordered mode ids, each skipped when unavailable by default.
- `channel_conditions`: named ordered channel chains.
- `seeds`: ordered deterministic seeds.
- `export_audio`: forwarded to single-run placeholder artifact recording.

ISSUE-0030 adds a minimal TOML-shaped loader for the existing sweep config files
and a user-facing `f700f-sweep` CLI. The loader supports the documented scalar
keys, generated-tone input, `[[modes]]`, `[[channel_conditions]]`, and
`[[channel_conditions.channel_chain]]` arrays of tables. Richer TOML schema work,
including includes/shared channel fragments and file-audio inputs, remains planned.

## Run Behavior

Runs are emitted in stable nested order:

1. mode order from the config
2. channel condition order from the config
3. seed order from the config

Each record has one status:

- `completed`: the underlying simulation completed successfully.
- `skipped`: the mode was not registered or not merged into the current build.
- `failed`: the mode was available, but the single simulation run failed.

Skipped or failed records do not stop later records. Invalid empty sweep configs
are rejected before any run starts.

## Artifacts

When `output_directory` is set, the runner writes:

- `<run_id_prefix>.json`: aggregate sweep records.
- `<run_id_prefix>.csv`: aggregate sweep records.
- `runs/<per-run-id>.json` and `runs/<per-run-id>.csv`: existing Module 11
  single-run summaries for completed or failed attempted simulations.

The aggregate CSV columns are:

`run_id,status,mode_id,condition_id,seed,simulation_ok,digest,skipped_reason,error_summary`

## M1 Baseline Smoke

`make_m1_baseline_smoke_sweep_config()` lists:

- `ssb_standard_3k`
- `ssb_narrow_1k9`
- `freedv700d_emulated`
- `freedv700e_emulated`
- `freedv700d_official`
- `freedv700e_official`

and channel conditions:

- identity
- AWGN high
- AWGN low
- frequency offset
- simple gain fading

Modes that are not present in the current build are recorded as skipped. With
`F700F_ENABLE_CODEC2=OFF`, official FreeDV modes are expected to skip unless a
future optional adapter registers them.

## M2 Channel Matrix Helpers

ISSUE-0025 adds channel-matrix config helpers without changing the runner expansion
contract:

- `make_m2_channel_matrix_smoke_sweep_config()` returns the CI-sized matrix:
  identity, AWGN 6 dB, AWGN 0 dB, seed 1.
- `make_m2_channel_matrix_full_sweep_config()` returns 72 channel conditions from
  AWGN SNR `-2, 0, 2, 4, 6, 8` dB, frequency offsets `0, 50, 100, 200` Hz, and
  fading profiles `none, weak, medium`, with seeds `1, 2, 3`.

The matrix details are specified in `docs/specs/channel_condition_matrix.md`, with
TOML-friendly shapes under `configs/channels/`.

The runner rejects duplicate channel condition ids before expansion and rejects empty
seed lists before starting runs. Channel factory adapters reject malformed numeric
parameters rather than silently substituting defaults.

## M2 700F Candidate Campaign Helpers

ISSUE-0026 adds campaign helpers that compose the ISSUE-0025 channel matrices with the
M2 baseline/candidate mode list:

- `make_m2_700f_candidate_smoke_sweep_config()` uses the smoke matrix, seed `1`, and
  run id prefix `m2-700f-candidate-smoke`.
- `make_m2_700f_candidate_full_sweep_config()` uses the full 72-condition matrix, seeds
  `1`, `2`, and `3`, and run id prefix `m2-700f-candidate-full`.

Both helpers use stable mode ordering: SSB standard/narrow, 700D/700E emulated,
700D/700E official skip-capable entries, and 700F-A/B/C surrogate candidates.
The campaign details are specified in `docs/specs/m2_sweep_campaign.md`, with
TOML-friendly shapes under `configs/sweeps/`.

ISSUE-0028 adds `register_m2_campaign_mode_factories()` for the default M2 smoke
bridge. It registers SSB reference factories, 700D/700E emulator factories, and
700F-A/B/C candidate factories. ISSUE-0032 marks 700F candidate records completed
with `surrogate_completed` notes plus `implementation_status = surrogate`,
`not_real_modem = true`, `downselect_valid = false`,
`not_downselect_valid = true`, and `performance_valid = false`.
ISSUE-0033 moves the 700D/700E emulator rows from descriptor-only completion into
deterministic `emulated_surrogate` runtime completion. These rows are marked
completed with an
`emulated_surrogate_completed` note in JSON/CSV summaries, including
`official=false`, `not_official_freedv=true`, `downselect_valid=false`,
`performance_valid=false`, `emulator_model_name`, `emulator_model_version`, and
`emulator_limitations`. They do not claim official FreeDV behavior or valid
performance evidence. 700F surrogate rows do not claim waveform encode/decode or
real performance metrics. Official FreeDV 700D/700E entries remain skipped in
default Codec2-disabled smoke runs with an
`official_freedv_codec2_unavailable` reason that records
`official=true`, `codec2_enabled=false`, `codec2_available=false`,
`roundtrip_available=false`, `not_emulator=true`, and `not_surrogate=true`.

ISSUE-0032 appends these aggregate row fields to JSON/CSV outputs while keeping
existing columns stable: `implementation_status`, `not_real_modem`,
`downselect_valid`, `not_downselect_valid`, `performance_valid`,
`surrogate_model_name`, `surrogate_model_version`, `surrogate_limitations`,
`surrogate_readiness_score_synthetic`, and `synthetic_metrics_label`.

## Sweep CLI

`f700f-sweep --config <path>` loads a supported sweep TOML file, registers the
default M2 smoke-capable factories, runs the sweep, and writes aggregate JSON/CSV
artifacts according to the sweep config. `--output-dir <path>` overrides
`output_directory`, and `--run-id <id>` overrides `run_id_prefix`.

With `F700F_ENABLE_CODEC2=OFF`, unavailable official FreeDV modes and unknown
skip-capable modes are recorded as skipped rows rather than failing the sweep.

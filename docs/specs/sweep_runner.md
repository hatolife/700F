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

TOML parsing is intentionally deferred. `configs/sweeps/m1_baseline_smoke.toml`
documents the intended file schema for the later parser.

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

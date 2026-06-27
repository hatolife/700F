# Simulation Pipeline v0.1.0

ISSUE-0009 introduces the deterministic simulation core used to run one local pipeline:

`input audio -> mode encode -> channel chain -> mode decode -> metrics/result collection`

The v0.1.0 implementation lives in Module 11 and depends on the Mode interface from
ISSUE-0008. It does not define the final channel model, metrics artifact format, 700F
optimization loop, SSB emulator, or Codec2/FreeDV adapter.

## Configuration Shape

`SimulationConfig` is a TOML-friendly value type:

- `run_id`: stable identifier for the run and generated artifacts.
- `seed`: deterministic seed passed to channel adapters.
- `input`: generated test tone or an audio file reference. Generated tones are implemented;
  file loading is reserved for audio IO integration.
- `mode_id`: Mode registry identifier.
- `channel_chain`: ordered list of channel ids and key/value parameters.
- `output_directory`: optional directory for summary artifacts.
- `export_audio`: records the intended decoded-audio artifact connection.
- `metric_ids`: ordered metric selection.

The same config and seed must produce the same decoded audio, metric values, and
deterministic digest for deterministic mode/channel/metric components.

## Stages

Every run records status for these stages:

- `input`: build or load audio.
- `encode`: configure and run the selected mode encoder.
- `channel`: apply the configured channel chain.
- `decode`: run the selected mode decoder.
- `metrics`: compute requested metrics and emit summaries.

Failures stop the pipeline at the failing stage, preserve prior stage statuses, and return a
human-readable error. Invalid mode ids, unknown channels, invalid channel parameters, and
unknown metrics are clear failures.

## Initial Adapters

The initial adapters are deliberately minimal:

- dummy mode: maps mono audio samples to complex samples and recovers the real component.
- identity channel: returns complex samples unchanged and accepts the run seed.
- dummy metric: mean absolute sample error between input and decoded audio.

These adapters provide a deterministic contract test bed while ISSUE-0010 and ISSUE-0014
define richer channel and result artifact contracts.

## Result And Artifacts

`SimulationResult` retains `run_id`, `seed`, `mode_id`, ordered channel ids, decoded audio,
metric values, stage statuses, artifacts, and a deterministic digest.

When `output_directory` and `run_id` are set, the runner emits summary JSON and CSV files
and records them as result artifacts. If `export_audio` is true, the result records the
intended decoded-audio artifact path as a placeholder; actual audio serialization belongs
to later audio/result artifact work.

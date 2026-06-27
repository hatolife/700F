# Module Spec

Module 11 owns the local end-to-end simulation runner.

## ISSUE-0009 Scope

The deterministic simulation core runs one configured pipeline:

`input audio -> mode encode -> channel chain -> mode decode -> metrics/result collection`

The implementation uses the Mode interface from `include/f700f/types.hpp` and keeps the
initial channel and metric adapters local to Module 11. Later channel and artifact work can
adapt this boundary without requiring a Mode interface change.

## Initial Components

- `SimulationRunner`: registry-backed coordinator for modes, channels, and metrics.
- `SimulationConfig`: TOML-friendly run config containing run id, seed, input, mode id,
  channel chain, output directory, export-audio flag, and metric selection.
- `SimulationResult`: metadata, deterministic digest, decoded audio, metric values, stage
  statuses, artifacts, and any failure message.
- Dummy adapters: dummy mode, identity channel, and dummy metric for deterministic tests.

# API Contract

The Module 11 C++ API is exposed by `f700f/simulation_pipeline.hpp`.

## Stable For M1

- `SimulationConfig` describes one run and is intentionally simple to map to TOML later.
- `SimulationRunner::run` returns a `SimulationResult` instead of throwing for invalid run
  configuration.
- `SimulationResult::stage_status` reports per-stage success/failure for input, encode,
  channel, decode, and metrics.
- `simulation_result_to_json` and `simulation_result_to_csv` include the
  decoded audio export path as `audio_export_path`.
- `simulation_result_to_json` and `simulation_result_to_csv` provide summary serialization
  for result artifact integration.

## Extension Points

- Modes use the ISSUE-0008 `IModeFactory` and `ModeRegistry`.
- Channels implement Module 11-local `IChannelFactory`/`IChannel`.
- Metrics implement Module 11-local `IMetric`.

These local channel and metric interfaces are intentionally small until ISSUE-0010 and
ISSUE-0014 define broader contracts.

# Occupied Bandwidth Validation Harness

## Status

- Issue: ISSUE-0047
- Target milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Implementation status: simulation harness

## Purpose

ISSUE-0047 adds a deterministic simulation-side occupied bandwidth estimate so
700F-A prototype runs can report a comparable bandwidth diagnostic before final
OFDM/FEC/sync behavior exists.

This measurement is engineering evidence only. It does not make 700F-A final,
interoperable, performance-valid, or downselect-valid.

## Measurement Method

The first harness uses `f700f::metrics::measure_occupied_bandwidth` from
`f700f/occupied_bandwidth.hpp`.

Inputs:

- complex baseband samples;
- positive sample rate in hertz;
- occupied energy ratio, default `0.99`.

The implementation computes deterministic DFT bin energy, sorts bins by
frequency, trims equal low/high tail energy, and reports the frequency span that
contains the requested energy ratio. Empty signals and zero-energy signals
return safe zero-width measurements with explicit status values. Invalid sample
rates and invalid ratios are rejected.

## Artifact Fields

Result and sweep/report paths may carry:

- `occupied_bandwidth_estimate_hz`
- `occupied_bandwidth_low_hz`
- `occupied_bandwidth_high_hz`
- `occupied_bandwidth_ratio`
- `occupied_bandwidth_status`

Reports compare the estimate with the descriptor target bandwidth. For M3-A the
700F-A planning target remains `1900 Hz`.

## Limitations

- This is not a regulatory occupied bandwidth measurement.
- The current implementation is a simple DFT harness, not a windowed PSD or
  calibrated spectrum analyzer.
- Short sample blocks and tones that are not bin-centered can broaden or smear
  the estimate.
- The harness measures simulation baseband samples only; it does not validate a
  transmitted RF waveform.
- Prototype rows remain `performance_valid=false`,
  `performance_validity=limited`, `downselect_valid=false`, and
  `downselect_validity=invalid`.

## Test Coverage

Contract tests cover empty signal safety, zero signal safety, invalid
sample-rate rejection, stable single-tone estimates, two-tone estimates wider
than single-tone estimates, result JSON/CSV propagation, and Markdown report
visibility with the 1.9 kHz target comparison.

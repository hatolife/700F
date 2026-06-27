# Data Protocol v0.1.0

ISSUE-0008 standardizes the data blocks exchanged at the Mode boundary. These structures
are C++ value types for local simulation code. They are not yet a serialized file format
or stable C ABI layout.

## Blocks

`AudioBlock` carries mono floating-point audio samples, a sample rate, and a start time in
seconds. Sample values are normalized by convention; exact clipping and calibration policy
belongs to later audio and simulation issues.

`ComplexBlock` carries complex baseband samples as `Complex32`, a sample rate, center
frequency in hertz, and a start time in seconds. It is the common waveform exchange type
for modems and future channel simulation.

`BitBlock` carries hard bits as bytes with values interpreted as 0 or 1. It includes a
frame index for associating payloads with frame-level status.

`SoftBitBlock` carries log-likelihood-like soft decisions as floats with a frame index.
v0.1.0 defines the container only; numeric scaling is intentionally left to later modem
and FEC work.

`FrameStatus` reports frame index, sync state, FEC success, confidence, corrected errors,
and erasures. It is diagnostic metadata and must not be treated as a complete scorecard.

## Result ownership

`EncodeResult` owns all encoded outputs for one call. A mode may populate symbols, bits,
or both depending on its descriptor capabilities.

`DecodeResult` owns all decoded outputs for one call. A mode may populate audio, bits, or
both depending on its descriptor capabilities.

When `ok` is false, consumers must treat output blocks as diagnostic only and inspect the
human-readable `error` string if one is supplied.

## Compatibility

The v0.1.0 data protocol is source-level C++ API. Future binary/plugin protocols must
translate these structures through explicit ABI structs and function tables rather than
exposing STL containers across a C boundary.

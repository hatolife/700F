# Modem Frame And Symbol Protocol v0.2.0

ISSUE-0043 adds source-level C++20 value types for the first real 700F modem
prototype boundary. The protocol is intentionally a container and metadata
contract only. It does not implement the 700F-A minimal QPSK-like waveform,
official FreeDV framing, FEC, synchronization, codec payload processing, or
downselect-valid performance evidence.

## Public C++ Surface

The public surface lives in `include/f700f/types.hpp` and is append-only relative
to the v0.1.0 Mode interface:

- `SampleRate` wraps a positive sample rate in hertz and exposes `valid()`.
- `SymbolRate` wraps a positive finite symbol rate in baud and exposes `valid()`.
- `CarrierLayout` is a placeholder carrier metadata container.
- `TimingSyncStatus` is a placeholder synchronization metadata container.
- `FecStatus` is a placeholder FEC metadata container.
- `CodecStatus` is a placeholder codec metadata container.
- `PrototypeFrameStatus` records prototype status, guardrails, and limitations.
- `ModemFrameDescriptor` carries common mode/frame/rate/family metadata and
  exposes `valid_rates()`.
- `ModemInputFrame` carries payload bytes and requested modem/FEC/sync/codec
  families entering a prototype modem boundary.
- `SymbolBlock` carries complex symbols, bits-per-symbol, family metadata, and
  limitations before sample generation.
- `BasebandFrame` carries complex baseband samples, sample/symbol rates,
  frequency/bandwidth metadata, family metadata, and limitations.
- `ModemOutputFrame` carries decoded payload bytes, prototype status,
  symbol-count diagnostics, prototype metrics, and limitations.

## Rate Validation

`SampleRate::valid()` returns true only when `hz > 0`.
`SymbolRate::valid()` returns true only when `baud` is finite and greater than
zero. `ModemFrameDescriptor::valid_rates()` requires both rates to be valid.

Invalid rates are detected by these helpers; constructors remain default and
non-throwing so zero-length and placeholder protocol objects are safe to create
and fill incrementally.

## Default And Zero-Length Behavior

All frame/block containers are default constructible. Empty payload, symbol,
sample, metric, and limitation vectors are valid container states and must not
imply waveform processing. Default-constructed descriptors have invalid rates
until a caller provides positive values.

`PrototypeFrameStatus` defaults to:

- `status = limited`
- `implementation_status = real_modem_prototype`
- `prototype = true`
- `not_final_modem = true`
- `downselect_valid = false`
- `not_downselect_valid = true`
- `performance_valid = limited`

These defaults keep prototype diagnostics visible while preserving the project
policy that real 700F downselect remains prohibited.

## Runtime Compatibility

ISSUE-0043 does not change `IMode`, `IModeFactory`, `ModeRegistry`,
`EncodeResult`, or `DecodeResult`. At the ISSUE-0043 boundary merge, existing
700F-A `toy_audio_waveform`, 700F-B/C surrogate, SSB, FreeDV emulator, and
optional Codec2 runtime behavior was unchanged. ISSUE-0042 later moves only
700F-A onto the minimal QPSK-like baseband prototype while leaving B/C
surrogate rows unchanged.

## Reporting Expectations

Downstream metrics and reporting code may copy these fields into future
prototype diagnostics, but final scoring must continue to treat prototype rows
as downselect-invalid and performance-limited until a later audit explicitly
changes that policy.

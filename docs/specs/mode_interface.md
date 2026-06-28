# Mode Interface v0.1.0

ISSUE-0008 defines the common C++20 Mode boundary used by simulation code to describe,
configure, encode, decode, and reset a candidate or reference mode. The interface is
intentionally small: it is a protocol contract, not a 700F implementation.

## Public C++ surface

The v0.1.0 public surface lives in `include/f700f/types.hpp`.

- `ModeId` is a stable string identifier such as `ssb.reference` or `freedv.700d`.
- `ModeCapabilities` records coarse IO features: audio, complex samples, bit payloads,
  soft bits, and variable frame sizing.
- `ModeDescriptor` describes a mode without constructing a runtime instance. It exposes
  bandwidth, latency, sample rate, codec name, FEC name, modem name, audio passband,
  frame timing, CP timing, nominal bit rates, carrier count, modulation family, pilot
  strategy, implementation status, and flags that distinguish emulator skeletons from
  official baselines.
- `ModeRuntimeConfig` carries runtime options needed before encode/decode. v0.1.0 only
  standardizes sample rate, max frame samples, and key/value parameters.
- `EncodeResult` returns generated complex symbols, optional bits, frame status, and an
  error string when `ok` is false.
- `DecodeResult` returns recovered audio, optional bits, frame status, and an error
  string when `ok` is false.
- `IMode` owns one runtime instance. Implementations must support `descriptor`,
  `configure`, `encode`, `decode`, and `reset`.
- `IModeFactory` exposes one descriptor and creates fresh `IMode` instances.
- `ModeRegistry` is an in-process registry keyed by `ModeId`.

## Runtime behavior

`configure` returns `false` when a requested runtime configuration is unsupported. A
failed configure call must leave the mode in a state where later encode/decode calls fail
gracefully with `ok == false`; implementations may recover through a later successful
configure call.

`reset` clears stateful counters, synchronizers, buffers, and history used by the mode
runtime. It must not mutate the descriptor.

Zero-length blocks are valid inputs. Implementations may return empty outputs, but they
must not crash or advance hidden buffers in a way that prevents later normal operation.

## Descriptor examples

An SSB-like descriptor can set `codec_id = "analog-voice"`, `fec_id = "none"`,
`modem_id = "ssb"`, an RF bandwidth around the voice passband, and capabilities for
audio input plus complex output.

A FreeDV-like descriptor can set `codec_id = "codec2"`, a FreeDV-family FEC identifier,
an OFDM modem identifier, nominal frame duration, CP duration, bitrate fields, carrier
count, modulation family, pilot strategy, and complex IO or bit payload capabilities.
Descriptor-only emulators must set `emulator = true`, `official_baseline = false`, and
an implementation status that makes unimplemented runtime behavior explicit. This
representation does not implement or validate any official FreeDV mode.

M2 700F candidate profiles may set `emulator = false`, `official_baseline = false`, and
`implementation_status = "profile_only"` when they are selectable architecture/profile
descriptors rather than emulator or official baseline implementations. Their encode and
decode calls must fail clearly until follow-up implementation issues replace the stub.

## Non-goals

v0.1.0 does not implement 700F, Codec2 adapters, SSB emulation, FreeDV 700D/700E
emulation, a channel model, persistence, plugin loading, or ABI-stable binary exchange.

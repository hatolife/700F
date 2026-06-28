# API Contract

Module 02 owns the v0.1.0 source-level data structures in `include/f700f/types.hpp`.

- Blocks: `AudioBlock`, `ComplexBlock`, `BitBlock`, and `SoftBitBlock`.
- Status: `FrameStatus`.
- Mode metadata: `ModeId`, `ModeCapabilities`, and `ModeDescriptor`.
- Runtime exchange: `ModeRuntimeConfig`, `EncodeResult`, and `DecodeResult`.
- Real-modem-prototype boundary: `SampleRate`, `SymbolRate`, `CarrierLayout`,
  `TimingSyncStatus`, `FecStatus`, `CodecStatus`, `PrototypeFrameStatus`,
  `ModemFrameDescriptor`, `ModemInputFrame`, `SymbolBlock`, `BasebandFrame`,
  and `ModemOutputFrame`.

The public protocol is documented in `docs/specs/data_protocol.md` and
`docs/specs/mode_interface.md`. The module does not implement concrete modes in
ISSUE-0008.

ISSUE-0032 uses the existing `ModeDescriptor::implementation_status` string to mark
700F candidate descriptors as `surrogate`; metrics/reporting modules carry the
append-only not-real-modem and not-downselect-valid fields documented in
`docs/specs/700f_candidate_surrogate_behavior.md`.

ISSUE-0043 adds default-constructible value containers and minimal rate
validation for the modem frame/symbol/baseband boundary. It does not change
`IMode`, `EncodeResult`, `DecodeResult`, or any current 700F runtime behavior.

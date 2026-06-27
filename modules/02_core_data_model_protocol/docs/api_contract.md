# API Contract

Module 02 owns the v0.1.0 source-level data structures in `include/f700f/types.hpp`.

- Blocks: `AudioBlock`, `ComplexBlock`, `BitBlock`, and `SoftBitBlock`.
- Status: `FrameStatus`.
- Mode metadata: `ModeId`, `ModeCapabilities`, and `ModeDescriptor`.
- Runtime exchange: `ModeRuntimeConfig`, `EncodeResult`, and `DecodeResult`.

The public protocol is documented in `docs/specs/data_protocol.md` and
`docs/specs/mode_interface.md`. The module does not implement concrete modes in
ISSUE-0008.

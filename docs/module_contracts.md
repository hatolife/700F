# Module Contracts

## M1 Mode Boundary

Module 02 owns the source-level data model for the v0.1.0 Mode interface:
`AudioBlock`, `ComplexBlock`, `BitBlock`, `SoftBitBlock`, `FrameStatus`,
`ModeDescriptor`, `ModeCapabilities`, `ModeRuntimeConfig`, `EncodeResult`, and
`DecodeResult`.

Module 03 owns the registry/plugin-facing boundary for the same Mode concept:
`IMode`, `IModeFactory`, `ModeRegistry`, and the future C ABI plugin protocol described
in `docs/specs/plugin_protocol.md`.

Downstream modules may depend on descriptors, block containers, and interface contracts,
but ISSUE-0008 does not grant permission to implement specific SSB, FreeDV, Codec2, 700F,
or channel behavior.

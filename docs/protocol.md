# Protocol

The M1 v0.1.0 source protocol introduces a common Mode boundary for local simulation
work. The normative v0.1.0 details are split by concern:

- `docs/specs/mode_interface.md` defines the C++20 `IMode`, `IModeFactory`, descriptor,
  runtime config, result, and registry surface.
- `docs/specs/data_protocol.md` defines the audio, complex, bit, soft-bit, and frame
  status value types exchanged at the boundary.
- `docs/specs/plugin_protocol.md` documents the future C ABI plugin relationship.

ISSUE-0008 is intentionally a protocol/interface issue. It does not implement 700F,
Codec2, SSB, FreeDV 700D/700E, or the channel model.

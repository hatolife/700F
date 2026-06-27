# API Contract

Module 03 owns the v0.1.0 source-level registry and factory contracts in
`include/f700f/types.hpp`.

- Runtime interface: `IMode`.
- Construction interface: `IModeFactory`.
- In-process registry: `ModeRegistry`.
- Future plugin boundary: `docs/specs/plugin_protocol.md`.

ISSUE-0008 does not implement dynamic plugin loading. The C ABI is documentation-only in
v0.1.0 and must not expose STL containers across the binary boundary when implemented.

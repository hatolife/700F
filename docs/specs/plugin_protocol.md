# Plugin Protocol v0.1.0

ISSUE-0008 documents the intended relationship between the in-process C++ Mode interface
and a future C ABI plugin protocol. v0.1.0 does not load shared libraries or expose a C
header; it defines the design line that later plugin work must preserve.

## Relationship to C++ Mode API

The C++ API is the native simulation-facing interface:

- host code discovers descriptors through `IModeFactory` and `ModeRegistry`;
- host code creates a fresh `IMode` runtime per simulation stream;
- encode/decode/reset calls operate on owned C++ value types.

A future C ABI plugin must be an adapter around the same concepts, not a competing mode
model. It should expose descriptor discovery, instance creation/destruction, configure,
encode, decode, reset, and error reporting.

## Future ABI requirements

The future C ABI must avoid STL types, C++ exceptions, and ownership ambiguity at the
binary boundary. It should use:

- fixed-size ABI/version fields;
- explicit struct sizes for forward-compatible extension;
- UTF-8 string pointers with documented lifetimes;
- caller-provided buffers or allocator callbacks for block payloads;
- status codes for every function;
- one opaque runtime handle per mode instance;
- one opaque factory or plugin handle per loaded module.

## Version identifiers

`versions.toml` keeps the protocol and plugin ABI labels. For v0.1.0 the C++ source
protocol is updated while the C ABI remains documentation-only. Later plugin ABI changes
must update the manifest and this spec together.

## Non-goals

v0.1.0 does not define shared object names, dynamic loader behavior, filesystem discovery,
signing, sandboxing, allocator ABI, or cross-language bindings.

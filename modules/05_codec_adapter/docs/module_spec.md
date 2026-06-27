# Module Spec

Module 05 owns optional adapters around external codec implementations.

## v0.1.0 scope

- Add discoverable official FreeDV 700D/700E baseline descriptors.
- Keep default builds independent of `external/codec2`.
- Use `F700F_ENABLE_CODEC2=ON` as the only path that includes pinned Codec2 FreeDV API
  headers.
- Distinguish unavailable Codec2 configuration, unsupported runtime configuration, and
  guarded runtime failures in returned error text.

Full Codec2 waveform encode/decode is outside v0.1.0 scope.

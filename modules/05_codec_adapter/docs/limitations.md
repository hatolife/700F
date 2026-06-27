# Limitations

- ISSUE-0012 adds official FreeDV 700D/700E descriptors and availability reporting, but
  full Codec2 waveform encode/decode binding is not implemented in v0.1.0.
- With `F700F_ENABLE_CODEC2=OFF`, official modes are discoverable and return explicit
  unavailable/skip errors rather than attempting to use Codec2.
- With `F700F_ENABLE_CODEC2=ON`, the adapter includes the pinned Codec2 FreeDV API header
  and compile-checks `FREEDV_MODE_700D`/`FREEDV_MODE_700E`; runtime encode/decode still
  reports a guarded skeleton failure until the full Codec2 binding issue lands.

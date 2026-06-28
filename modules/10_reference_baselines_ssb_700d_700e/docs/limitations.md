# Limitations

- ISSUE-0033 moves `freedv700d_emulated` and `freedv700e_emulated` beyond
  descriptor-only rows into deterministic `emulated_surrogate` behavior. They
  are emulator modes, not official Codec2/FreeDV baselines.
- Encode and decode use a deterministic audio-to-IQ/audio surrogate for plumbing
  tests only. No Codec2 adapter, official FreeDV modem, FEC, interleaver,
  synchronization, BER/FER validation, or official validation baseline is
  implemented. Reports must carry `not_official_freedv=true`,
  `performance_valid=false`, and `downselect_valid=false` semantics.
- ISSUE-0012 remains the owner for any official 700D/700E baseline adapter work.
- ISSUE-0011 adds `ssb_standard_3k` and `ssb_narrow_1k9` reference placeholders.
  These modes are audio-only descriptors with simple bandwidth-limited pass-through
  runtime behavior; they are not high-fidelity SSB emulators.

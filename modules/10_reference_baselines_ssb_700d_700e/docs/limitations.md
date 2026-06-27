# Limitations

- ISSUE-0013 adds descriptor-only `freedv700d_emulated` and `freedv700e_emulated`
  skeletons. They are emulator modes, not official Codec2/FreeDV baselines.
- Encode and decode return explicit "not implemented" failures. No Codec2 adapter,
  official FreeDV modem, FEC, interleaver, synchronization, or validation baseline is
  implemented.
- ISSUE-0012 remains the owner for any official 700D/700E baseline adapter work.

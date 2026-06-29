# Limitations

- ISSUE-0013 records OFDM descriptor assumptions for the FreeDV 700D/700E emulator
  skeletons only. Carrier counts, CP duration, modulation family, and pilot strategy are
  placeholders and are not official FreeDV parameters.
- No OFDM modulator, demodulator, carrier mapper, pilot inserter, synchronizer, or
  high-fidelity waveform engine is implemented.
- ISSUE-0042 implements only a minimal QPSK-like baseband prototype for
  `freedv700f_a_balanced`. It uses synthetic PCM-byte payloads, no FEC,
  `sync_family=none`, no Codec2 payload integration, no final synchronization,
  and no real downselect-valid performance evidence.
- ISSUE-0046 adds OFDM carrier-layout metadata only. The 19-carrier, 100 Hz
  spacing, 1.9 kHz occupied-bandwidth target layout is a prototype planning
  descriptor with data, pilot, and guard placeholders; it is not a final
  waveform design and does not validate occupied bandwidth.

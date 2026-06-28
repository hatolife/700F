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

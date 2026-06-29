# Module 07: OFDM Modem and Waveform Engine

Module 07 owns prototype modem and waveform behavior for 700F candidate work.

Current implemented scope:

- ISSUE-0042 minimal QPSK-like baseband prototype for
  `freedv700f_a_balanced`.
- ISSUE-0046 OFDM carrier-layout prototype metadata and validation helpers for
  M3 planning.
- Explicit non-final guardrails: no OFDM, no FEC, no final synchronization, no
  Codec2 payload integration, and no real downselect-valid performance
  evidence.

700F-B/C remain surrogate rows until later issues explicitly upgrade them.

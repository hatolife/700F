# Limitations

- ISSUE-0043 defines protocol containers only. It does not implement the
  700F-A QPSK-like prototype, OFDM, FEC, synchronization, codec payload
  processing, or official FreeDV interoperability.
- Rate validation is intentionally minimal: sample rates must be positive and
  symbol rates must be positive finite values. Detailed modem profile
  compatibility checks belong to later modem implementation issues.
- The new prototype status defaults are diagnostic guardrails, not performance
  evidence. Real 700F downselect remains prohibited until a later audit.

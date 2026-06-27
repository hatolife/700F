# Limitations

- ISSUE-0009 provides dummy mode, identity channel, and dummy metric adapters only.
- Audio file input is represented in config but not loaded yet; generated test tone input is
  implemented.
- Decoded audio export records an artifact path placeholder when requested, but raw/WAV
  serialization is deferred to later audio/result artifact work.
- Channel behavior is deterministic and identity-only until ISSUE-0010 lands the channel
  model.
- Metrics are limited to a dummy mean absolute error metric until ISSUE-0014 defines the
  result artifact and scoring format.

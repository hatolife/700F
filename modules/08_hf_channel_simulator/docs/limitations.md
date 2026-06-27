# Limitations

- No multipath, OFDM-like distortion, or Doppler spread model is included.
- AWGN, offset, and simple gain are block-level primitives only.
- Frequency offset currently accepts audio blocks as passthrough for test-path compatibility.
- Advanced channel descriptors (Rayleigh/Rician/multipath) are tracked in `docs/specs/channel_models.md` as future work.

# Testing

- `ctest --test-dir build --output-on-failure`
- `tests/test_channel_models.cpp` validates:
  - seed determinism and seed sensitivity for AWGN
  - SNR effect on added noise
  - 0 Hz frequency offset no-op behavior
  - seeded gain fading
  - ordered channel chain behavior
  - zero-length block handling
  - invalid sample-rate rejection in frequency offset

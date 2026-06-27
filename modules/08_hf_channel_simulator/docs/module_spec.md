# Module 08: HF Channel Simulator

## Purpose

Provide minimal channel impairments used by deterministic simulation workflows:
AWGN, frequency offset, and simple gain fading.

## Public behavior

- `f700f::AwgnChannel`
  - supports `f700f::AudioBlock` and `f700f::ComplexBlock`
  - uses SNR-to-noise variance scaling and seedable RNG
- `f700f::FrequencyOffsetChannel`
  - applies sample-rate-based complex phase rotation
  - rejects invalid sample rates
- `f700f::SimpleGainFadingChannel`
  - applies one sampled block gain from configurable dB bounds
  - reproducible when re-seeded
- `f700f::ChannelChain`
  - applies effects in the order added

## Minimal scope

- No multipath, no codec, and no stateful channel simulation beyond seed-backed random draws.
- No plugin ABI is added in this issue.

# Channel Models Spec

## Scope

Module 08 implements a minimal reproducible RF channel chain for M1 simulations.

## Implemented models (v0.1.0)

- `AwgnChannel`
  - Supports both `f700f::AudioBlock` and `f700f::ComplexBlock`.
  - Uses seedable `std::mt19937_64` RNG for reproducibility.
  - Uses the requested SNR (dB) to scale Gaussian noise power to the signal power.
- `FrequencyOffsetChannel`
  - Applies a fixed frequency shift to `f700f::ComplexBlock`.
  - Uses block sample rate for phase step (`2π f_off / sample_rate`).
  - Rejects zero sample rate as invalid.
  - Audio blocks are accepted as passthrough for pipeline compatibility.
- `SimpleGainFadingChannel`
  - Applies block-wise complex or real gain sampled from a seedable uniform dB range.
  - Reproducible with seed control.
- `ChannelChain`
  - Holds ordered channel effects in `add()` order.
  - Applies effects in sequence without reordering.

## Known limitations for v0.1.0

- The simple gain model is currently one gain coefficient per block.
- Frequency offset is single-tone and memoryless per block.
- AWGN is zero-noise for empty or zero-power blocks.
- Future work:
  - high-fidelity Rayleigh/Rician/multipath fading
  - channel state continuity across blocks
  - burst errors and IQ clipping behavior
  - external channel profile ingestion


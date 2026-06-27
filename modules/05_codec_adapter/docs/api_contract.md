# API Contract

Module 05 exposes `f700f/codec_adapter/freedv_official.hpp`.

## FreeDV official adapter

- `freedv700d_official_descriptor()` and `freedv700e_official_descriptor()` return static
  `ModeDescriptor` instances for the official Codec2/FreeDV baselines.
- Official descriptors use mode ids `freedv700d_official` and `freedv700e_official`, set
  `official_baseline=true`, and set `emulator=false`.
- `register_freedv_official_modes(ModeRegistry&)` registers factories for both official
  modes.
- `freedv_official_codec2_available()` reports whether the adapter was built with
  `F700F_ENABLE_CODEC2=ON`.
- `freedv_official_codec2_mode_id(ModeId)` returns pinned Codec2 API ids for the official
  modes: 700D is `7`, 700E is `13`, unknown mode ids return `-1`.

With Codec2 disabled, runtime `configure`, `encode`, and `decode` fail gracefully with an
unavailable reason. With Codec2 enabled, v0.1.0 compile-checks the official API ids but
still reports guarded runtime failures for waveform encode/decode.

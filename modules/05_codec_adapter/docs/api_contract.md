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
unavailable reason. With Codec2 enabled, the adapter links the pinned upstream `codec2`
runtime target, opens official 700D/700E FreeDV TX/RX handles, converts F700F float audio
to Codec2 short/`COMP` samples, and uses complex TX/RX for Mode-boundary smoke coverage.
This is official baseline runtime evidence, not real 700F candidate downselect evidence.

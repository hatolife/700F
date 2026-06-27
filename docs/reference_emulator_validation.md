# Reference Emulator Validation

ISSUE-0013 validates only Mode interface availability for the parameterized FreeDV
emulator skeletons:

- `freedv700d_emulated` and `freedv700e_emulated` are discoverable descriptors.
- Both modes can be registered and selected through `ModeRegistry`.
- Both modes are explicitly marked `emulator = true` and `official_baseline = false`.
- Descriptor fields cover RF bandwidth, audio bandwidth, frame duration, CP duration,
  raw bitrate, carrier count, modulation family, FEC name, pilot strategy, and
  implementation status.
- Runtime encode/decode calls return clear "not implemented" failures.

This validation does not compare against Codec2/FreeDV output, waveform spectra,
decoder synchronization, FEC performance, or official 700D/700E behavior.

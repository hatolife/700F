# Limitations

- ISSUE-0012 adds official FreeDV 700D/700E descriptors and availability reporting.
- With `F700F_ENABLE_CODEC2=OFF`, official modes are discoverable and return explicit
  unavailable/skip errors rather than attempting to use Codec2.
- With `F700F_ENABLE_CODEC2=ON`, ISSUE-0036 links the pinned Codec2 runtime and supports
  official 700D/700E Mode-boundary smoke encode/decode.
- The Codec2-on path is still optional and non-default; recurring Codec2-on CI, broader
  waveform validation, and real 700F candidate downselect remain follow-up work.

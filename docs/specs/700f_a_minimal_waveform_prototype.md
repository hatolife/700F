# 700F-A Minimal Waveform Prototype

## Status

- Issue: ISSUE-0039
- Branch: `feature/ISSUE-0039-700f-a-minimal-waveform-prototype`
- Depends: ISSUE-0038

## Scope

ISSUE-0039 implements the first waveform-capable 700F candidate path for
`freedv700f_a_balanced` only. The prototype is a deterministic
`toy_audio_waveform` path that maps audio samples to complex samples and back
through the existing simulation pipeline.

`freedv700f_b_robust` and `freedv700f_c_quality` remain ISSUE-0032 surrogate
rows.

## Guardrail Metadata

700F-A rows must expose:

- `implementation_status = waveform_prototype`
- `prototype = true`
- `not_final_modem = true`
- `waveform_capable = true`
- `downselect_valid = false`
- `not_downselect_valid = true`
- `performance_valid = false`
- `codec_family = synthetic`
- `fec_family = none`
- `modem_family = toy_audio_waveform`
- `prototype_limitations` stating that the row is synthetic, non-final,
  non-official, and not valid for real downselect.

The prototype can complete encode/channel/decode/metrics in the smoke pipeline,
but those completed rows are readiness evidence only.

## Limitations

The prototype is not interoperable FreeDV 700F. It does not implement Codec2,
official FreeDV framing, OFDM, FEC, interleaving, synchronization, or a final
modem. Real candidate downselect remains prohibited.

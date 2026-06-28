# 700F-A Minimal QPSK Baseband Prototype

## Status

- Issue: ISSUE-0042
- Branch: `feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype`
- Depends: ISSUE-0043, ISSUE-0044
- Implementation status: `real_modem_prototype`

## Scope

ISSUE-0042 moves only `freedv700f_a_balanced` from the ISSUE-0039
`toy_audio_waveform` path to a minimal QPSK-like baseband prototype. The
prototype uses the ISSUE-0043 frame, symbol, baseband, and status containers.

`freedv700f_b_robust` and `freedv700f_c_quality` remain ISSUE-0032 surrogate
rows.

## Prototype Boundary

The public helper path is:

- `make_700f_a_minimal_qpsk_input_frame()`
- `encode_700f_a_minimal_qpsk_symbols()`
- `modulate_700f_a_minimal_qpsk_baseband()`
- `demodulate_700f_a_minimal_qpsk_baseband()`

The helper path converts synthetic PCM-byte payloads into QPSK-like hard
symbols, carries those symbols as one complex baseband sample per symbol, and
demodulates them back to synthetic payload bytes. It is deterministic and
zero-length safe. Invalid sample or symbol rates are rejected with explicit
limitations.

## Metadata

700F-A rows expose:

- `implementation_status = real_modem_prototype`
- `implementation_classification = real_modem_prototype`
- `prototype = true`
- `not_final_modem = true`
- `waveform_capable = true`
- `downselect_valid = false`
- `not_downselect_valid = true`
- `performance_valid = false`
- `performance_validity = limited`
- `downselect_validity = invalid`
- `codec_family = synthetic`
- `fec_family = none`
- `sync_family = none`
- `modem_family = minimal_qpsk`

Reports also include `performance_valid=limited` in the prototype warning text
for readability while preserving the ISSUE-0044 boolean compatibility field
`performance_valid=false`.

## Diagnostics

Sweep and report artifacts may carry:

- `prototype_frame_status = completed`
- `prototype_sync_status = none`
- `prototype_baseband_sample_count`
- `prototype_limitations`

BER and FER remain `N/A`/unavailable for real performance scoring. Prototype
diagnostics are engineering-only evidence and must not be used for real 700F
candidate ranking.

## Limitations

This is not final FreeDV 700F and is not interoperable with upstream FreeDV. It
does not implement Codec2, official FreeDV framing, OFDM, FEC, interleaving,
timing recovery, carrier synchronization, pilot design, or final modem
performance validation. Real 700F downselect remains prohibited.

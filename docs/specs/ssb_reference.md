# SSB Reference Modes (M1)

This spec defines the reference-mode placeholders added for ISSUE-0011.

## Mode identifiers

- `ssb_standard_3k`
- `ssb_narrow_1k9`

Both modes are audio-leaning analog reference emulators and are not official baselines.
They are registered as emulator modes and discoverable through `ModeRegistry`.

## Descriptor requirements

Both modes are represented with `ModeDescriptor` fields that communicate:

- codec family as `analog-voice`
- no FEC (`fec_id = "none"`)
- no bit payload output (`capabilities.bit_payload = false`)
- `emulator = true`
- `official_baseline = false`
- audio bandwidth and occupied RF bandwidth metadata
- nominal latency estimate

### `ssb_standard_3k`

- occupied bandwidth (`rf_bandwidth_hz`) is wider than the narrow variant
- audio passband is around 300–3300 Hz
- audio bandwidth is provided for test/metric selection and selection checks

### `ssb_narrow_1k9`

- occupied bandwidth is narrower than `ssb_standard_3k`
- audio passband is narrower than the standard mode
- codec, modem, and timing metadata remain consistent with an analog placeholder

## Runtime behavior

No high-fidelity channel or SSB modulation is implemented in this issue.
The encode/decode implementation uses a placeholder path that preserves block shape and
applies a small, deterministic low-pass-like limiter to approximate passband behavior.
Zero-length audio blocks are valid and return empty outputs without error.

## Status markers

- `implementation_status` for both modes mentions the ISSUE-0011 placeholder intent and
  indicates that BER/FER are represented through metadata (`bit_payload = false`, zero bitrates).

## Module limitations

- BER/FER metrics remain represented as not-applicable for these modes in the artifact level.
- Export of decoded audio artifacts remains represented as existing pipeline placeholder behavior
  until dedicated audio I/O work is added in Module 04.

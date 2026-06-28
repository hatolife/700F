# FreeDV Emulator Behavior Spec v0.2.0

ISSUE-0033 defines the minimal behavior bridge for the non-official
`freedv700d_emulated` and `freedv700e_emulated` modes.

## Scope

The emulator modes are deterministic surrogates for M2 tooling only. They are
not Codec2/FreeDV implementations and must not be used as official baseline or
downselect performance evidence.

## Runtime Behavior

Both emulator modes:

- expose `implementation_status = "emulated_surrogate"`;
- keep `official_baseline = false` and `emulator = true`;
- configure only the descriptor sample rate or an unspecified sample rate;
- encode audio into a deterministic complex stream with a simple
  bandwidth-limited audio mapping;
- decode the real component of that complex stream back into deterministic
  audio;
- expose deterministic placeholder bit payloads for plumbing tests only.

The model metadata reported by sweep/report paths is:

- `official=false`
- `not_official_freedv=true`
- `downselect_valid=false`
- `performance_valid=false`
- `emulator_model_name=f700f-minimal-freedv700d700e-surrogate`
- `emulator_model_version=ISSUE-0033-v1`
- `emulator_limitations=not official FreeDV; performance_valid=false; downselect_valid=false; deterministic audio-to-IQ/audio surrogate only; no Codec2, FreeDV modem, FEC, interleaver, synchronization, or official validation`

## Limitations

The surrogate does not implement Codec2, a FreeDV modem, FEC, interleaving,
synchronization, bit-error measurement, frame-error measurement, or official
FreeDV validation. Reports must label these rows as non-official and
performance-invalid even when the simulation row status is `completed`.

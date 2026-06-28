# 700F Candidate Surrogate Behavior v0.2.0

ISSUE-0032 moves the three M2 700F candidates from profile-only sweep rows to
minimal surrogate behavior rows. The surrogate exists only to exercise campaign,
artifact, scoring, and reporting paths before a real modem is implemented.

## Scope

The surrogate applies to:

- `freedv700f_a_balanced`
- `freedv700f_b_robust`
- `freedv700f_c_quality`

Each candidate descriptor uses `implementation_status = "surrogate"`. Runtime
encode/decode still fail clearly with ISSUE-0032 text; the sweep runner completes
these modes through a metadata/surrogate bridge without running waveform encode,
channel decode, FEC, sync, BER, or FER.

## Required Guardrails

Surrogate rows and reports must expose these fields or equivalent text:

- `implementation_status = surrogate`
- `not_real_modem = true`
- `downselect_valid = false`
- `not_downselect_valid = true`
- `performance_valid = false`
- `surrogate_model_name = 700f_candidate_minimal_behavior`
- `surrogate_model_version = ISSUE-0032-v1`
- `surrogate_limitations` explaining that the row is synthetic readiness only,
  not a real modem, and does not emit BER/FER as real values.

Synthetic metrics, when present, must carry a synthetic label. The ISSUE-0032
readiness slot is `surrogate_readiness_score_synthetic`; it is separate from real
RF/audio performance scoring.

## Scoring Boundary

Completed surrogate rows count as completed sweep rows and as surrogate evidence.
They do not contribute to real performance score, BER availability, FER availability,
or real candidate downselect feasibility. Reports must keep the real performance
score separate from surrogate readiness score and show a surrogate warning.

## Limitations

The surrogate is not interoperable FreeDV behavior. It does not encode or decode a
700F waveform, validate Codec2 parameters, exercise FEC, estimate sync, or measure
RF/audio performance. Real candidate downselect remains prohibited until waveform-
capable candidates and official baseline roundtrip evidence exist.

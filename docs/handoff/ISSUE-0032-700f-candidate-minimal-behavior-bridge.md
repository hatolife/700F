# ISSUE-0032 700F Candidate Minimal Behavior Bridge Handoff

## Status

- Branch: `feature/ISSUE-0032-700f-candidate-minimal-behavior`
- Worktree: `../700F-ISSUE-0032`
- Local CI: `bash ./tools/run_ci_local.sh` passed on 2026-06-29.
- Push/PR: not performed.

## Implemented Behavior

- `freedv700f_a_balanced`, `freedv700f_b_robust`, and
  `freedv700f_c_quality` use `implementation_status = "surrogate"`.
- Sweep rows complete as `surrogate_completed` with `not_real_modem = true`,
  `downselect_valid = false`, `not_downselect_valid = true`, and
  `performance_valid = false`.
- Surrogate metadata is emitted as `surrogate_model_name`,
  `surrogate_model_version`, and `surrogate_limitations`.
- Synthetic readiness, when present, is labeled
  `surrogate_readiness_score_synthetic` and
  `synthetic_surrogate_readiness_only`.

## Reporting And Scoring Boundary

- Reports emit a surrogate warning and keep real performance score separate from
  synthetic surrogate readiness score.
- BER/FER are not emitted or scored as real surrogate performance values.
- Real candidate downselect remains prohibited.

## Remaining Blockers

- ISSUE-0033 must still define 700D/700E emulator minimal behavior without official
  claims.
- ISSUE-0034 must still provide official FreeDV waveform roundtrip evidence.
- Real 700F waveform/FEC/sync implementations remain under ISSUE-0019 through
  ISSUE-0021.

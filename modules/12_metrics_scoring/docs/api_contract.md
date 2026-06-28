# API Contract

## Result Artifacts

Module 12 owns `f700f/metrics_artifacts.hpp`, including the append-only
`ResultArtifact` JSON/CSV helpers documented in `docs/specs/metrics.md`.

## M2 Scoring

ISSUE-0024 adds `f700f/scoring_model.hpp`.

- `make_m2_score_policy()` returns the default deterministic policy
  `m2-interim-v0`.
- `score_m2_results(results, profile_snapshots, policy)` aggregates result artifacts
  and optional profile snapshots into per-mode scores.
- `M2ScoreReport::find_mode(mode_id)` returns a stable pointer to a mode score or
  `nullptr`.
- `m2_score_report_to_json(report)` emits a compact JSON summary for reporting
  consumers.

The scorer infers completed/skipped/failed state from `skipped_reason` and
`error_summary`; callers do not pass a separate status enum.

ISSUE-0032 extends descriptor snapshots append-only with surrogate guardrail fields:
`not_real_modem`, `downselect_valid`, `not_downselect_valid`, `performance_valid`,
`surrogate_model_name`, `surrogate_model_version`, and `surrogate_limitations`.
Completed surrogate rows can carry `surrogate_readiness_score_synthetic`, but real
performance score remains separate and excludes rows whose `performance_valid` is
false.

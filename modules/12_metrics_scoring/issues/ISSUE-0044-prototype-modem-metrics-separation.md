# ISSUE-0044: prototype modem metrics separation

## Status
done

## Scope
module:12

## Priority
P0

## Owner
- Codex role: Module 12 + Module 15 + Module 13

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0044-prototype-modem-metrics-separation
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up

## Task
Separate prototype modem diagnostics from final performance and downselect
scoring.

## Acceptance criteria
- [x] Prototype metrics are visible.
- [x] Prototype metrics do not affect final downselect ranking.
- [x] OFF and ON CI pass.

## TDD record
- 2026-06-29: Started with metrics artifact, scoring, and report contract tests
  for `real_modem_prototype` separation before implementation changes.
- 2026-06-29: Added passing contract tests for artifact JSON/CSV round-trip,
  scoring counters, Markdown warnings, and report loader classification.

## Validation

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with explicit WSL
  git worktree environment; transient Codec2 ON build/report artifacts cleaned.

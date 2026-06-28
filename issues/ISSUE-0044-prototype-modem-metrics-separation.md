# ISSUE-0044: prototype modem metrics separation

- Status: complete
- Priority: P0
- Scope: module:12,15,13
- Owner: Module 12 + Module 15 + Module 13
- Branch: feature/ISSUE-0044-prototype-modem-metrics-separation
- Target version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up
- Depends: ISSUE-0041, ISSUE-0043
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

Real modem prototype diagnostics must be visible without becoming final scoring
evidence. Metrics, reports, and sweep artifacts need a stable separation between
prototype diagnostics and performance-valid downselect data.

## Acceptance Criteria

- [x] Add or reserve fields for prototype modem diagnostics such as symbol error
  rate, frame status, sync status, and limitations.
- [x] Keep `downselect_valid = false` for prototype rows.
- [x] Keep `performance_valid = limited` separate from final performance scores.
- [x] Update report rendering to show prototype diagnostics without ranking.
- [x] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- 2026-06-29: Started on
  `feature/ISSUE-0044-prototype-modem-metrics-separation` in worktree
  `C:/Users/user/Documents/700F-ISSUE-0044`.
- Planned first failing tests: metrics artifact prototype diagnostic round-trip,
  scoring separation for `real_modem_prototype` rows, and report warnings that
  show prototype diagnostics without ranking them as real performance.
- 2026-06-29: Added and passed metrics artifact, scoring, report renderer, and
  report loader contract tests for `real_modem_prototype` limited diagnostics.

## Validation

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed: build, 20/20 CTest,
  version check, and governance check.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with explicit WSL
  `GIT_DIR`/`GIT_COMMON_DIR` worktree environment: 20/20 CTest, 27/27 smoke rows,
  and 6 official FreeDV rows validated. Generated `build-codec2` and transient
  ISSUE-0037 report artifacts were cleaned afterward.

## Notes

This issue may run in parallel with ISSUE-0043 if it avoids source protocol
types until they land.

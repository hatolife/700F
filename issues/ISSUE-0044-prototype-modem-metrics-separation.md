# ISSUE-0044: prototype modem metrics separation

- Status: open
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

- [ ] Add or reserve fields for prototype modem diagnostics such as symbol error
  rate, frame status, sync status, and limitations.
- [ ] Keep `downselect_valid = false` for prototype rows.
- [ ] Keep `performance_valid = limited` separate from final performance scores.
- [ ] Update report rendering to show prototype diagnostics without ranking.
- [ ] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Pending. Start with metrics/report loader contract tests.

## Notes

This issue may run in parallel with ISSUE-0043 if it avoids source protocol
types until they land.

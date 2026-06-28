# API Contract

## M2 Baseline Comparison Report

ISSUE-0027 adds a source-level C++20 Markdown renderer in
`include/f700f/baseline_comparison_report.hpp`.

Public entry points:

- `M2BaselineComparisonReportContext`: report metadata and sweep context.
- `render_m2_baseline_comparison_report(context, score_report)`: renders deterministic
  Markdown from ISSUE-0024 `M2ScoreReport`.
- `make_m2_baseline_report_filename(run_id, timestamp_utc)`: returns a sanitized,
  timestamped Markdown filename.

The renderer does not own scoring policy. `f700f::metrics::M2ScoreReport` remains the
source of truth for mode ordering, score values, counters, penalties, profile-only
status, skipped counts, and official-unavailable counts.

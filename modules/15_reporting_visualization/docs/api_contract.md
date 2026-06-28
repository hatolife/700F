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
source of truth for mode ordering, score values, counters, penalties, surrogate
status/readiness, profile-only compatibility status, skipped counts, and
official-unavailable counts.

## Report Artifact Loader And CLI

ISSUE-0031 adds `include/f700f/report_artifact_loader.hpp` and the
`f700f-report` app.

Public entry points:

- `load_report_input_json(json_payload)`: loads sweep aggregate JSON rows.
- `load_report_input_csv(csv_payload)`: loads sweep aggregate CSV rows.
- `load_report_input_file(path)`: dispatches by `.json` or `.csv` extension.
- `render_report_from_loaded_input(loaded)`: scores loaded rows and renders Markdown.

The loader maps sweep rows into reporting/scoring artifacts and labels
`surrogate`, `profile_only`, `descriptor_only`, skipped, and failed rows as
non-performance evidence. The generated report states whether real downselect is
possible, prints a surrogate warning for 700F surrogate rows, and keeps real
performance score separate from synthetic surrogate readiness score.

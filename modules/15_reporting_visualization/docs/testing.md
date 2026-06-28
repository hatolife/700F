# Testing

- `test_baseline_comparison_report` covers deterministic Markdown rendering for empty
  and minimal score inputs, skipped/unavailable official FreeDV records, profile-only
  700F candidates, raw metric summaries, blocked/missing sweep context, and timestamped
  filename generation.
- `test_report_artifact_loader_cli` covers sweep aggregate JSON/CSV loading, malformed
  input errors, Markdown generation, skipped rows, profile-only rows, descriptor-only
  rows, and downselect feasibility wording.
- `f700f_report_help`, `f700f_report_missing_input`, and
  `f700f_report_cli_contract` cover CLI help, missing input, malformed JSON/CSV, and
  sample JSON Markdown generation.
- Full repository verification uses `bash ./tools/run_ci_local.sh`.

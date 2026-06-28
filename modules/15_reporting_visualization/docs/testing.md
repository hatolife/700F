# Testing

- `test_baseline_comparison_report` covers deterministic Markdown rendering for empty
  and minimal score inputs, skipped/unavailable official FreeDV records, profile-only
  700F candidates, raw metric summaries, blocked/missing sweep context, and timestamped
  filename generation.
- Full repository verification uses `bash ./tools/run_ci_local.sh`.

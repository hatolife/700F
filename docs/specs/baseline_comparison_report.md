# M2 Baseline Comparison Report v0.2.0

ISSUE-0027 defines deterministic Markdown report generation over ISSUE-0024 M2
scoring output. The report is a comparison aid for M2 candidate exploration; it does
not declare a final 700F downselect.

## Inputs

The renderer consumes:

- `M2ScoreReport` from `f700f::metrics::score_m2_results`;
- report context containing run id, commit hash, config path, generation timestamp,
  sweep execution status, ordered mode ids, ordered channel ids, ordered seeds, known
  limitations, and a next-recommendation placeholder.

This keeps the scoring output as the source of truth for score tables, status counts,
raw metric summaries, profile-only candidates, skipped modes, and official-unavailable
FreeDV baselines. Sweep execution can still be described as blocked or missing by
setting the context `sweep_status` string, which is expected while ISSUE-0026 is not
available or has not produced artifacts.

## Required Markdown Sections

Generated reports include:

- run metadata: run id, commit hash, config path, generation timestamp, scoring policy,
  total record count, and sweep status;
- mode list, channel list, and seeds from report context;
- score table sorted by the scorer's deterministic score order;
- skipped and unavailable modes;
- raw metrics summary with dropout, latency, bandwidth, BER/FER, ASR WER, STOI,
  ESTOI, and subjective-note slot counts;
- baseline and candidate status for official FreeDV, SSB references, and 700F
  candidates;
- known limitations;
- next recommendation placeholder.

## Empty And Minimal Inputs

Empty scoring output renders deterministically with explicit placeholder text instead
of omitting sections. Minimal context fields render as `unknown` or `none recorded`.

## Timestamped Filename

`make_m2_baseline_report_filename(run_id, timestamp_utc)` returns:

```text
m2-baseline-comparison-<sanitized-run-id>-<compact-timestamp>.md
```

The run id is restricted to alphanumeric, `_`, and `-` characters with other bytes
replaced by `_`. The timestamp keeps only alphanumeric characters so an ISO-8601 value
such as `2026-06-28T12:34:56Z` becomes `20260628T123456Z`.

## Limitations

- The report renderer does not parse sweep TOML, run a sweep, or read artifacts from
  disk. ISSUE-0026 owns sweep execution and artifact production.
- Official FreeDV availability is summarized from scored records whose descriptor has
  `official_baseline = true` and skipped/unavailable counts.
- 700F candidate entries with `implementation_status = "profile_only"` are displayed
  as non-performance evidence with score `0.0` until later implementation issues
  produce completed runs.

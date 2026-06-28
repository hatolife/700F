# Module Spec

Module 15 owns reporting and visualization surfaces that consume validated artifacts
from upstream modules without redefining scoring or simulation semantics.

## M2 Baseline Comparison Report

ISSUE-0027 provides a deterministic Markdown renderer for M2 baseline comparison
reports. It consumes Module 12 `M2ScoreReport` output plus explicit report context for
run metadata, config path, channel/mode/seed lists, sweep execution status, known
limitations, and next-recommendation text.

The report includes:

- run id, commit hash, config path, generation timestamp, scoring policy, record count,
  and sweep status;
- mode list, channel list, and seeds;
- score table and raw metrics summary;
- skipped/unavailable mode summary;
- official FreeDV, SSB reference, and 700F candidate status;
- known limitations and next recommendation placeholder.

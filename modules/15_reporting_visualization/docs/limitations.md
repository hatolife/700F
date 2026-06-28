# Limitations

- ISSUE-0027 report generation renders Markdown from already-built scoring output; it
  does not run sweeps, parse TOML, or load JSON/CSV artifacts from disk.
- Missing or blocked sweep execution must be represented through report context
  `sweep_status`.
- The report preserves profile-only 700F candidates as non-performance evidence until
  later implementation issues produce completed candidate runs.
- Official FreeDV status is only as complete as the scored records provided by Module
  12; with default `F700F_ENABLE_CODEC2=OFF`, official modes are expected to appear as
  skipped/unavailable if included in input records.

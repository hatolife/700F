# Limitations

- ISSUE-0027 report generation renders Markdown from already-built scoring output; it
  does not run sweeps or parse TOML.
- ISSUE-0031 can load sweep aggregate JSON/CSV artifacts from disk, but it does not
  run sweeps or inspect per-run audio/RF payloads.
- Missing or blocked sweep execution must be represented through report context
  `sweep_status`.
- The report preserves ISSUE-0032 700F surrogate candidates as not-real-modem,
  not-downselect-valid, non-performance evidence until later implementation issues
  produce waveform-capable candidate runs.
- ISSUE-0044 real modem prototype rows are displayed as limited diagnostics with
  warnings and continue to block real downselect.
- ISSUE-0047 occupied bandwidth values are displayed as simulation diagnostics
  only; the report does not treat them as final RF compliance evidence.
- Descriptor-only emulator rows are also non-performance evidence and block real
  downselect.
- Official FreeDV status is only as complete as the scored records provided by Module
  12; with default `F700F_ENABLE_CODEC2=OFF`, official modes are expected to appear as
  skipped/unavailable if included in input records.

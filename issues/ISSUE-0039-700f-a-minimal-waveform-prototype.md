# ISSUE-0039: 700F-A minimal waveform-capable prototype

- Status: complete
- Priority: P0
- Scope: module:02,07,11,12,13,15
- Owner: Module 02 + Module 07 + Module 13 + Module 15
- Branch: feature/ISSUE-0039-700f-a-minimal-waveform-prototype
- Target version: v0.2.0 target
- Milestone: M2-G waveform prototype
- Depends: ISSUE-0038
- Last updated: 2026-06-29

## Problem

ISSUE-0038 defines a guarded path for the first waveform-capable 700F prototype.
The repository now needs the smallest implementation that lets one 700F candidate
exercise encode/channel/decode through the existing simulation pipeline while
keeping all prototype claims clearly non-final.

## Acceptance Criteria

- [x] Only `freedv700f_a_balanced` changes from surrogate to
  `implementation_status = waveform_prototype`.
- [x] `freedv700f_b_robust` and `freedv700f_c_quality` remain ISSUE-0032
  surrogate rows.
- [x] 700F-A Mode-boundary encode/decode is deterministic and does not crash on
  zero-length or short generated audio.
- [x] Sweep JSON/CSV expose prototype metadata:
  `prototype = true`, `not_final_modem = true`, `waveform_capable = true`,
  `downselect_valid = false`, `performance_valid = false`,
  `codec_family = synthetic`, `fec_family = none`, `modem_family`, and
  prototype limitations.
- [x] Reports/scoring keep prototype rows separate from real performance and
  keep real 700F downselect prohibited.
- [x] Local Codec2 OFF and Codec2 ON CI pass.

## TDD record

- 2026-06-29: Started from `develop` after ISSUE-0038 merge and OFF/ON local CI
  success. First tests will require 700F-A descriptor/runtime prototype behavior
  and sweep/report metadata while asserting 700F-B/C remain surrogate.
- 2026-06-29: Red test confirmed `SweepRunRecord` lacked prototype metadata
  fields. Implementation added 700F-A toy waveform runtime, prototype
  serialization, report/scoring guardrails, and documentation.
- 2026-06-29: `ctest --test-dir build --output-on-failure` passed 20/20 after
  implementation.
- 2026-06-29: M2-G smoke completed 21 rows, skipped 6 official Codec2-OFF rows,
  failed 0, and generated `reports/20260629_052411_issue0039_m2g_comparison.md`.
- 2026-06-29: `bash ./tools/run_ci_local.sh` passed with 20/20 CTest, version
  check, and governance check. Clock-skew warnings were observed and remain
  non-blocking per ISSUE-0035.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with 20/20 Codec2
  ON CTest, 27/27 smoke rows, and 6 validated official FreeDV rows.

## Result

Completed. `freedv700f_a_balanced` is now a deterministic toy waveform prototype
with `implementation_status = waveform_prototype`, `prototype = true`,
`not_final_modem = true`, `waveform_capable = true`, `codec_family = synthetic`,
`fec_family = none`, and `modem_family = toy_audio_waveform`. 700F-B/C remain
ISSUE-0032 surrogate rows. Reports and scoring keep prototype rows
performance-invalid and downselect-invalid. Real 700F downselect remains
prohibited.

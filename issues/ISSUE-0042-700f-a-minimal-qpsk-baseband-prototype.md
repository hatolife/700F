# ISSUE-0042: 700F-A minimal QPSK baseband prototype

- Status: complete
- Priority: P0
- Scope: module:07,02,11,12,13,15
- Owner: Module 07 + Module 02 + Module 13 + Module 15
- Branch: feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype
- Target version: v0.2.0 target
- Milestone: M2 real modem prototype follow-up
- Depends: ISSUE-0041, ISSUE-0043
- Recommended model: gpt-5.5-xhigh
- Last updated: 2026-06-29

## Problem

ISSUE-0041 selected Option B as the first implementation step after the toy
waveform prototype. 700F-A needs a minimal QPSK-like baseband prototype that uses
the RFC boundary metadata without claiming final modem performance.

## Acceptance Criteria

- [x] Implement only `freedv700f_a_balanced` as
  `implementation_status = real_modem_prototype`.
- [x] Preserve 700F-B and 700F-C as surrogate rows.
- [x] Use ISSUE-0043 frame/symbol protocol boundaries when available.
- [x] Emit `prototype = true`, `not_final_modem = true`,
  `downselect_valid = false`, and `performance_valid = limited`.
- [x] Keep FEC/sync/codec placeholders explicit in metadata and reports.
- [x] Keep Codec2 OFF local CI green.
- [x] Keep Codec2 ON local CI green and preserve official 700D/700E rows.

## TDD Record

- 2026-06-29: Subagent C started from branch
  `feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype`.
- Planned red tests before implementation:
  - `freedv700f_a_balanced` descriptor/snapshot resolves as
    `real_modem_prototype` with minimal QPSK metadata.
  - ISSUE-0043 `SymbolBlock` and `BasebandFrame` helpers generate QPSK-like
    prototype data and reject invalid sample/symbol rates.
  - Zero-length encode/decode is safe.
  - Identity and AWGN smoke rows complete while remaining
    downselect-invalid and performance-limited.
  - BER/FER remain non-real performance evidence and reports warn that real
    downselect is prohibited.
  - 700F-B/C remain surrogate rows.
- 2026-06-29: Red tests first exposed missing 700F-A minimal QPSK helpers and
  incomplete `sync_family` metadata propagation. Implementation then added the
  minimal QPSK-like SymbolBlock/BasebandFrame path, sweep/report metadata, and
  docs.
- 2026-06-29: Focused candidate, sweep, metrics, scoring, report loader, and
  baseline report tests passed locally.
- 2026-06-29: M2-H smoke/report passed: `f700f-sweep` completed 21 rows,
  skipped 6 Codec2-OFF official rows, failed 0, and `f700f-report` showed the
  700F-A real prototype warning, `performance_valid=limited`,
  `sync_family=none`, `modem_family=minimal_qpsk`, and real downselect
  prohibited.
- 2026-06-29: `bash ./tools/run_ci_local.sh` passed with 21/21 CTest, version
  check, and governance check. Clock-skew warnings were observed and remain
  non-blocking per ISSUE-0035.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with 21/21 Codec2
  ON CTest, 27/27 smoke rows, and 6 validated official FreeDV rows. The WSL
  invocation used explicit `GIT_DIR`/`GIT_WORK_TREE` because this sibling
  worktree stores its gitdir pointer in Windows path form.

## Result

Completed. `freedv700f_a_balanced` now uses a guarded minimal QPSK-like
baseband prototype with `implementation_status = real_modem_prototype`,
`prototype = true`, `not_final_modem = true`, `waveform_capable = true`,
`performance_valid = limited`, `downselect_valid = false`,
`modem_family = minimal_qpsk`, `codec_family = synthetic`,
`fec_family = none`, and `sync_family = none`. 700F-B/C remain ISSUE-0032
surrogate rows. BER/FER remain limited prototype diagnostics, not real
performance evidence. Real 700F downselect remains prohibited.

## Notes

Do not implement OFDM/FEC/sync final behavior here. Do not perform downselect.

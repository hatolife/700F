# Handoff: ISSUE-0042 through ISSUE-0045 QPSK Prototype and M3 Planning

- Date: 2026-06-29
- Current branch: `develop`
- Merge target: `develop`
- Completed issues: ISSUE-0043, ISSUE-0044, ISSUE-0042
- Open follow-up: ISSUE-0045
- Remote: `origin` configured; work branches and `develop` are pushable from this workspace.

## Current State

ISSUE-0043, ISSUE-0044, and ISSUE-0042 have been merged into `develop`.
ISSUE-0043 supplies the source-level modem frame/symbol/baseband protocol
containers, ISSUE-0044 prevents prototype diagnostics from becoming final
performance/downselect evidence, and ISSUE-0042 upgrades only
`freedv700f_a_balanced` to a minimal QPSK-like baseband prototype.

700F-A is now a limited real-modem prototype row, not a final modem:

- `prototype = true`
- `not_final_modem = true`
- `performance_validity = limited`
- `downselect_valid = false`
- `downselect_validity = invalid`

700F-B and 700F-C remain surrogate rows. Real 700F downselect remains blocked.

## Verification

- Local CI: `bash ./tools/run_ci_local.sh` passed with 21/21 CTest tests,
  version check, and governance check.
- Codec2 ON CI local: `bash ./tools/run_codec2_on_ci_local.sh` passed with
  21/21 CTest tests, 27/27 smoke rows, and 6/6 official FreeDV rows.
- M2-H Codec2 OFF smoke/report: 21 completed, 6 official FreeDV skips, 0 failed.
- M2-H Codec2 ON smoke/report: 27 completed, 0 skipped, 0 failed.

Reports:

- `reports/20260629_080853_m2h_700f_qpsk_comparison.md`
- `reports/20260629_080853_m2h_700f_qpsk_comparison_codec2_on.md`
- `reports/20260629_081841_m2h_qpsk_prototype.md`
- `docs/coordination/m2h_qpsk_prototype_integration_report.md`

## Next Work

ISSUE-0045 is startable as planning-only work on branch
`docs/ISSUE-0045-m3-planning-ofdm-fec-sync`. Recommended model:
`gpt-5.5-xhigh`.

ISSUE-0045 should define the M3 issue map and acceptance gates for OFDM, FEC,
and sync expansion. It should not implement those systems. It should carry
forward the rule that 700F-A QPSK prototype results are not downselect-valid and
cannot be used to claim superiority over SSB, 700D, or 700E.

## Blocked Items

- Real 700F candidate downselect remains blocked.
- 700F-B and 700F-C real modem work remains blocked until explicit follow-up
  issues upgrade them.
- OFDM/FEC/sync implementation remains blocked until ISSUE-0045 or a later M3
  planning issue authorizes scoped implementation.

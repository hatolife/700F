# ISSUE-0045: M3 planning for OFDM/FEC/sync

- Status: complete
- Priority: P1
- Scope: qa,project,module:06,07,09
- Owner: Manager + QA + Module 06 + Module 07 + Module 09
- Branch: docs/ISSUE-0045-m3-planning-ofdm-fec-sync
- Target version: v0.3.0 target
- Milestone: M3 planning
- Depends: ISSUE-0041, ISSUE-0042, ISSUE-0043, ISSUE-0044
- Recommended model: gpt-5.5-xhigh
- Last updated: 2026-06-29

## Problem

ISSUE-0041 recommends Option C after the minimal QPSK-like boundary is stable.
M3 needs planning for OFDM, FEC, and sync work without starting those larger
implementation tracks prematurely.

## Acceptance Criteria

- [x] Define M3 issue map for OFDM/FEC/sync expansion.
- [x] Identify dependencies on ISSUE-0042, ISSUE-0043, and ISSUE-0044.
- [x] Preserve no-downselect policy until a later audit.
- [x] Define candidate acceptance criteria for performance-valid evidence.
- [x] Keep planning separate from implementation.
- [x] Record M3 target version and affected modules in `versions.toml`.
- [x] Keep local Codec2 OFF and Codec2 ON CI green.

## TDD Record

- TDD exception: planning issue.
- 2026-06-29: Phase 0 passed on `develop`: `bash ./tools/run_ci_local.sh`
  passed with 21/21 CTest, version check, and governance check; `bash
  ./tools/run_codec2_on_ci_local.sh` passed with 21/21 CTest, 27/27 Codec2 ON
  smoke rows, and 6 official FreeDV rows.
- 2026-06-29: Created M3 milestone/spec/dispatch/handoff docs, root follow-up
  issues ISSUE-0046 through ISSUE-0053, and module-local stubs for the first M3
  owners. Planning kept real downselect prohibited and did not implement
  OFDM/FEC/sync runtime behavior.
- 2026-06-29: Branch verification passed: `bash ./tools/run_ci_local.sh` passed
  with 21/21 CTest, version check, and governance check; `bash
  ./tools/run_codec2_on_ci_local.sh` passed with 21/21 CTest, 27/27 Codec2 ON
  smoke rows, and 6 official FreeDV rows. Codec2 ON transient ISSUE-0037
  artifacts and `build-codec2` were cleaned afterward.

## Notes

Do not start M3 implementation until the M2 real prototype boundaries are merged
and audited.

## Result

Completed as planning-only. M3-A may start with low-risk boundary and regression
work: ISSUE-0046, ISSUE-0047, ISSUE-0049, and ISSUE-0051. M3-B and audit work
remain gated: ISSUE-0048, ISSUE-0050, ISSUE-0052, and ISSUE-0053.

# ISSUE-0045: M3 planning for OFDM/FEC/sync

- Status: open
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

- [ ] Define M3 issue map for OFDM/FEC/sync expansion.
- [ ] Identify dependencies on ISSUE-0042, ISSUE-0043, and ISSUE-0044.
- [ ] Preserve no-downselect policy until a later audit.
- [ ] Define candidate acceptance criteria for performance-valid evidence.
- [ ] Keep planning separate from implementation.

## TDD Record

- TDD exception: planning issue.

## Notes

Do not start M3 implementation until the M2 real prototype boundaries are merged
and audited.

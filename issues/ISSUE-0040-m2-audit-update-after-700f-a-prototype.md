# ISSUE-0040: M2 audit update after 700F-A prototype

- Status: complete
- Priority: P0
- Scope: qa, project
- Owner: Manager + QA
- Branch: chore/ISSUE-0040-m2-audit-update-after-700f-a-prototype
- Target version: v0.2.0 target
- Milestone: M2 real modem RFC gate
- Depends: ISSUE-0037, ISSUE-0038, ISSUE-0039
- Last updated: 2026-06-29

## Problem

ISSUE-0039 added the first guarded 700F-A toy waveform prototype after the
ISSUE-0038 RFC. M2 now needs an updated audit decision that records the
prototype state, confirms both Codec2 OFF and Codec2 ON checks remain green, and
decides whether the next step should be a real 700F modem architecture RFC.

## Acceptance Criteria

- [x] Confirm 700F-A is a toy waveform prototype, not a real modem.
- [x] Confirm 700F-B and 700F-C remain surrogate rows.
- [x] Confirm official 700D/700E Codec2 ON regression smoke remains available.
- [x] Confirm default Codec2 OFF local CI passes.
- [x] Confirm Codec2 ON local CI passes.
- [x] Confirm real 700F downselect remains prohibited.
- [x] Record root and Module 16 QA audit reports.
- [x] Record M2 decision and next issue direction.

## TDD Record

- TDD exception: audit/report-only issue. No production behavior is implemented.
- 2026-06-29: Pre-audit `bash ./tools/run_ci_local.sh` passed with 20/20 CTest,
  version check, and governance check.
- 2026-06-29: Pre-audit `bash ./tools/run_codec2_on_ci_local.sh` passed with
  20/20 Codec2 ON CTest, 27/27 smoke rows, and 6 validated official FreeDV rows.

## Result

Decision: `M2_GO_TO_REAL_MODEM_RFC`.

M2 may proceed to a real 700F modem architecture RFC. It must not perform real
candidate downselect yet because 700F-A is still a toy audio waveform prototype
with no real FEC, no timing/sync acquisition, no codec payload integration, and
no proof against final 700F targets. 700F-B and 700F-C remain surrogate rows.

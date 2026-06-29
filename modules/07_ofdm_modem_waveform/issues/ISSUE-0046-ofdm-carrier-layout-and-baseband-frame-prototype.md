# ISSUE-0046: OFDM carrier layout and baseband frame prototype

- Status: closed
- Priority: P0
- Scope: module:07
- Owner: Module 07 + Module 02 + Module 11 + Module 13
- Branch: feature/ISSUE-0046-ofdm-carrier-layout-baseband-frame
- Target version: v0.3.0 target
- Last updated: 2026-06-29

## Module Scope

Module 07 owns the OFDM carrier layout and baseband frame prototype boundary.
This issue must not replace the ISSUE-0042 minimal QPSK runtime with final OFDM.

## Acceptance Criteria

- [x] Root ISSUE-0046 acceptance criteria relevant to Module 07 are satisfied.
- [x] Carrier layout metadata remains prototype-only and downselect-invalid.
- [x] Codec2 OFF and Codec2 ON local CI remain green.

## TDD Record

- Planned red tests are recorded in root ISSUE-0046.
- 2026-06-29: Subagent A started with red tests for OFDM carrier-layout
  descriptor creation, invalid layout rejection, and baseband-frame placeholder
  metadata propagation.
- 2026-06-29: Red test failed on the missing Module 07 carrier-layout header;
  green after adding descriptor helpers, validation, and
  `ofdm_carrier_layout_contract`.

## Verification

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed, including 22/22 CTest.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with WSL-style
  `GIT_DIR`/`GIT_WORK_TREE` environment for this Windows worktree; transient
  Codec2 ON build/report artifacts were cleaned after verification.

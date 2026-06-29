# ISSUE-0049: FEC placeholder and UEP policy RFC

- Status: closed
- Priority: P0
- Scope: module:06,02,12,15
- Owner: Module 06 + Module 02 + Module 12 + Module 15
- Branch: docs/ISSUE-0049-fec-placeholder-uep-policy-rfc
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0044
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

M3 needs a replaceable FEC placeholder and UEP policy boundary before real FEC
or interleaver work starts.

## Acceptance Criteria

- [x] Define no-FEC, placeholder-FEC, and real-FEC states.
- [x] Define UEP policy metadata and future important-bit classes.
- [x] Document latency and interleaver relationships.
- [x] Describe metrics/report representation.
- [x] Keep FEC implementation and real downselect out of scope.
- [x] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planning/RFC-first. If source descriptors are added, start with tests for
  placeholder descriptor creation, `fec_family=placeholder`, no-FEC vs
  placeholder distinction, and UEP metadata retention.
- 2026-06-29: Started as RFC/documentation work. No source descriptor changes
  are planned unless the final diff shows a small, low-risk gap.
- 2026-06-29: Completed as documentation-only RFC work. No source descriptors
  were added, so descriptor tests were not required.

## Completion Notes

- `docs/specs/700f_fec_uep_policy_plan.md` is now the ISSUE-0049 policy source
  for no-FEC, placeholder-FEC, future real-FEC, UEP bit classes,
  latency/interleaver references, report representation, and downselect
  prohibition.
- Module 06 docs and task handoff records were updated to keep real FEC,
  interleaver behavior, and final Codec2 bit-importance mapping in future
  issues.
- Local CI passed on 2026-06-29:
  - `bash ./tools/run_ci_local.sh`: passed, 21/21 CTest tests plus version and
    governance checks.
  - `bash ./tools/run_codec2_on_ci_local.sh`: passed with WSL
    `GIT_DIR`/`GIT_WORK_TREE` set for this Windows-created worktree, 21/21
    CTest tests, 27 completed smoke rows, and 6 official FreeDV rows validated.
  - Codec2 ON transient `build-codec2` and generated report were removed after
    verification; tracked smoke fixtures were restored.

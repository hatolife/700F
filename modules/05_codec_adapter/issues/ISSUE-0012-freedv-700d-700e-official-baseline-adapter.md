# ISSUE-0012: FreeDV 700D 700E official baseline adapter

## Status
open

## Scope
module:05_codec_adapter

## Priority
P0

## Owner
- Codex role: Module 05
- Root tracking issue: ISSUE-0012

## Branch
- Base branch: develop
- Work branch: see root ISSUE-0012
- Worktree: ../f700f-wt-issue-0012
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target
- Module version: v0.1.0 target if implementation touches this module
- Compatibility line: 1
- Milestone: M1 Baseline Simulation

## Task
Module-local planning and implementation handoff for root ISSUE-0012: FreeDV 700D 700E official baseline adapter.

## Acceptance criteria
- [x] Root ISSUE-0012 acceptance criteria relevant to this module are satisfied, except
  full Codec2 waveform runtime binding deferred as documented in the root issue.
- [x] Module docs/issues/reports indexes are updated as work proceeds.
- [x] Module tests or documented TDD exception are added before implementation handoff closes.

## Test plan
- ./tools/run_ci_local.sh
- TDD red test before implementation: add `test_freedv_official_adapter` covering
  official descriptors, official/emulator flags, Codec2-disabled unavailable reason,
  guarded encode/decode skip errors, and Codec2-enabled smoke coverage only when the
  guarded build is available.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 issue expansion.
- 2026-06-28 JST M1-C subagent B started implementation work. Codec2 submodule
  initialized at `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`; authoritative mode ids
  are `FREEDV_MODE_700D=7` and `FREEDV_MODE_700E=13` from
  `external/codec2/src/freedv_api.h`.
- 2026-06-28 JST Added `f700f_codec_adapter`, official 700D/700E descriptors, guarded
  unavailable/runtime failure behavior, docs, and `test_freedv_official_adapter`.
- 2026-06-28 JST `bash ./tools/run_ci_local.sh` passed with 7 CTest tests plus version
  and governance checks.

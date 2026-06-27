# ISSUE-0008: Common Mode interface and data protocol v0.1.0

## Status
completed-local

## Scope
module:02_core_data_model_protocol

## Priority
P0

## Owner
- Codex role: Module 02
- Root tracking issue: ISSUE-0008

## Branch
- Base branch: develop
- Work branch: see root ISSUE-0008
- Worktree: ../f700f-wt-issue-0008
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target
- Module version: v0.1.0 target if implementation touches this module
- Compatibility line: 1
- Milestone: M1 Baseline Simulation

## Task
Module-local planning and implementation handoff for root ISSUE-0008: Common Mode interface and data protocol v0.1.0.

## Acceptance criteria
- [x] Root ISSUE-0008 acceptance criteria relevant to this module are satisfied.
- [x] Module docs/issues/reports indexes are updated as work proceeds.
- [x] Module tests or documented TDD exception are added before implementation handoff closes.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test target: `tests/test_mode_interface.cpp`
- Required cases: dummy descriptor retrieval, dummy encode/decode, reset, invalid config, zero-length block, SSB-like descriptor representation, and FreeDV-like descriptor representation.
- Red command: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ctest --test-dir build --output-on-failure`
- Expected failure before implementation: confirmed under bash; public Mode interface symbols are not available in `include/f700f/types.hpp`.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 issue expansion.
- 2026-06-27 23:40 JST Subagent C began ISSUE-0008 implementation for common data model/protocol interface.
- 2026-06-28 00:05 JST Completed local v0.1.0 data model contract with passing `bash ./tools/run_ci_local.sh`.

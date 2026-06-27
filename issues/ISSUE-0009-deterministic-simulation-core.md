# ISSUE-0009: Deterministic simulation core

## Status
open

## Scope
simulation

## Priority
P0

## Owner
- Codex role: Module 11 + Module 02 + Module 12
- Recommended model: gpt-5.5-medium

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0009-deterministic-simulation-core
- Worktree: ../f700f-wt-issue-0009
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 11, 02, 12 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create deterministic seed-fixed simulation runner and mode -> channel -> receiver -> metrics pipeline.

## Affected paths
- modules/11_pipeline_runner/
- docs/specs/simulation_pipeline.md
- tests/integration/

## Dependencies
- ISSUE-0008

## Acceptance criteria
- [x] Same config and seed produce same result.
- [x] Mode can be swapped.
- [x] Channel can be swapped.
- [x] Metrics can be swapped.
- [x] Minimal config can run one simulation.
- [x] Result can be emitted as JSON or CSV.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: planned `test_simulation_pipeline` covering dummy mode + identity channel success, deterministic same-seed output, invalid mode id, invalid channel config, zero-length audio, result metadata, and reflected stage failure before implementation.
- Red command: `bash -lc 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ctest --test-dir build --output-on-failure'`.
- Expected failure: `tests/test_simulation_pipeline.cpp:1:10: fatal error: f700f/simulation_pipeline.hpp: No such file or directory`.
- Green command: `bash ./tools/run_ci_local.sh` passed with 3/3 CTest tests, version check, and governance check.
- Refactor notes: keep channel and metrics as Module 11-local minimal abstractions for ISSUE-0009; defer public channel/result artifact contracts to ISSUE-0010/ISSUE-0014 integration.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-27 15:00 UTC M1-B subagent D started implementation branch and recorded red-test plan before code changes.
- 2026-06-27 15:05 UTC Red test failed as expected on missing `f700f/simulation_pipeline.hpp`.
- 2026-06-27 15:30 UTC Implemented Module 11 deterministic simulation core, added spec/docs, and passed local CI.

## Result
Local implementation complete on `feature/ISSUE-0009-deterministic-simulation-core`.
Push/PR intentionally not performed per task instruction. Deferred items: real audio file IO,
ISSUE-0010 channel model integration, ISSUE-0014 metrics/result artifact format integration,
and actual decoded-audio serialization.

# ISSUE-0015: Sweep runner v0.1.0

## Status
done

## Scope
module:13

## Priority
P0

## Owner
- Codex role: Module 13 + Module 11 + Module 15
- Recommended model: gpt-5.5-medium

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0015-sweep-runner-v0-1
- Worktree: ../f700f-wt-issue-0015
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 13, 11, 15 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create config-driven sweep runner for modes, channel conditions, and seeds.

## Affected paths
- modules/13_sweep_optimization/
- configs/sweeps/m1_baseline.toml
- docs/specs/sweep_runner.md

## Dependencies
- ISSUE-0009, ISSUE-0014

## Acceptance criteria
- [x] SSB standard, SSB narrow, 700D official-or-stub, and 700E official-or-stub can be listed in one sweep config.
- [x] Results are saved under reports or artifacts.
- [x] Seed-fixed runs are reproducible.
- [x] Codec2-disabled environment skips official 700D/700E with skip reason.
- [x] Short CI sweep exists.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: planned `tests/test_sweep_runner.cpp` covering empty config rejection, mode/channel/seed expansion, skipped modes, non-stopping failed runs, JSON/CSV aggregate output, stable ordering, and short smoke completion.
- Red command: `cmake --build build --target test_sweep_runner` after adding the test before implementation.
- Expected failure: target/header missing until Module 13 sweep runner library and CMake wiring are implemented. PowerShell PATH lacked `cmake`; Bash CMake is available and will be used for green verification.
- Green command: `bash -lc './build/test_sweep_runner'` passed; `bash ./tools/run_ci_local.sh` passed with 7/7 tests plus version and governance checks.
- Refactor notes: kept TOML parsing deferred; runner accepts `SweepConfig` and writes aggregate artifacts while delegating single runs to Module 11.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-28 00:00 JST M1-C subagent C started implementation in `../f700f-wt-issue-0015`; recorded red-test plan before code changes.
- 2026-06-28 00:00 JST Added Module 13 sweep runner, contract tests, aggregate JSON/CSV output, smoke TOML schema, and sweep runner spec.

## Result
Implemented initial sweep runner. Missing/unmerged modes are skipped; attempted failed runs are recorded and do not stop later runs. TOML parsing, official Codec2 adapter behavior, and real SSB/FreeDV waveform fidelity remain outside ISSUE-0015.

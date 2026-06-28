# ISSUE-0035: clock-skew warning investigation

## Status
complete

## Scope
module:01,build

## Priority
P2

## Owner
- Codex role: Module 01 + Manager
- Recommended model: GPT-5.3-Codex-Spark

## Branch
- Base branch: develop
- Work branch: chore/ISSUE-0035-clock-skew-warning-investigation
- Worktree: ../700F-ISSUE-0035
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0023

## Task
Investigate recurring local build clock-skew warnings. This is low priority
because build and tests pass.

## Acceptance criteria
- [x] Warning source is documented.
- [x] No risky build-system churn is introduced.
- [x] Local CI passes.

## TDD record
- TDD exception: investigation/documentation issue unless code changes are made.

## Result
- Warning source is reproducible on this worktree with `./tools/run_ci_local.sh`,
  on CMake-generated files under `build/CMakeFiles/*`.
- Causes are consistent with WSL `/mnt/c` + `drvfs` timestamp behavior and
  generated build metadata order effects.
- Warnings are non-blocking and CI still passes (`100% tests`, version and
governance checks passing).
- Supporting note added: `docs/troubleshooting/clock_skew_warnings.md`.
- Integrated locally into `develop` during M2-E. Clock-skew warnings remain
  non-blocking when CMake build, CTest, version check, and governance check pass.

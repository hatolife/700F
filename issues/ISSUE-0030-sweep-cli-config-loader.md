# ISSUE-0030: sweep CLI and config loader

## Status
done

## Scope
module:13,project

## Priority
P0

## Owner
- Codex role: Module 13 + Manager
- Recommended model: gpt-5.5-medium; fallback gpt-5.5-xhigh

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0030-sweep-cli-config-loader
- Worktree: ../700F-ISSUE-0030
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0023
- ISSUE-0028

## Task
Add a user-facing sweep CLI and config loader so M2 smoke can run outside the test
binary.

## Acceptance criteria
- [x] CLI help succeeds.
- [x] Missing config fails clearly.
- [x] Smoke config generates artifacts.
- [x] Output directory override works.
- [x] Run id override works.
- [x] Unknown/unavailable modes can be skipped.
- [x] TOML planned schema is documented if the first loader is simpler.
- [x] Local CI passes.

## Test plan
- Add red tests before implementation.
- `bash ./tools/run_ci_local.sh`

## TDD record
- 2026-06-29 JST: Starting on
  `feature/ISSUE-0030-sweep-cli-config-loader` from `develop`.
- Planned red tests: CLI `--help`, missing config error, smoke artifact
  generation, output directory override, run id override, and skip-capable
  unknown/unavailable modes.
- Red result: `bash -lc 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug'`
  failed because the new CLI tests reference missing target `f700f-sweep`.
- Green result: `bash -lc './tools/run_ci_local.sh'` passed with 16/16 CTest
  tests plus version and governance checks. Build emitted known non-blocking
  clock-skew warnings.

## Result
Implemented `f700f-sweep`, a minimal TOML-shaped sweep config loader, CLI
override handling, CLI smoke/missing-config/help tests, and documentation of the
supported subset plus planned richer schema work.

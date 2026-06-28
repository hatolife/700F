# ISSUE-0030: sweep CLI and config loader

## Status
done

## Scope
module:13

## Branch
- Work branch: feature/ISSUE-0030-sweep-cli-config-loader

## Task
Add a sweep CLI/config loader entry point for M2 smoke outside the test binary.

## Acceptance criteria
- [x] `--help` succeeds.
- [x] Missing config fails clearly.
- [x] Smoke config produces artifacts.
- [x] Output directory override works.
- [x] Run id override works.
- [x] Unknown modes remain skip-capable.
- [x] TOML planned schema is documented if the first loader is simpler.
- [x] Local CI passes.

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
Implemented `f700f-sweep`, minimal sweep TOML loading, CLI overrides, and
focused CLI/config loader tests. Richer TOML schema work remains planned and
documented.

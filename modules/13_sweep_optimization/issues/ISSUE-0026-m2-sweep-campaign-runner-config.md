# ISSUE-0026: M2 sweep campaign runner config

## Status
done

## Scope
Module 13 support for root ISSUE-0026.

## Branch
- Work branch: feature/ISSUE-0026-m2-sweep-campaign-config
- Merge target: develop

## Target version
v0.2.0 target

## Acceptance criteria
- Smoke and full campaign helpers/configs exist.
- Campaign mode ordering includes SSB standard/narrow, 700D/700E emulated,
  official 700D/700E skip-capable entries, and 700F-A/B/C profile-only candidates.
- Smoke path emits JSON/CSV aggregate artifacts quickly.
- Unknown or unavailable modes record skip reasons.
- Stable ordering is verified for the same seed/config.

## TDD record
- Red test added in root `tests/test_sweep_runner.cpp`.
- Red result: `bash -lc 'cmake --build build --target test_sweep_runner'` failed
  because `make_m2_700f_candidate_smoke_sweep_config` was missing.

## Notes
- The campaign helpers compose existing ISSUE-0025 matrix helpers and do not add TOML
  parsing or new simulation behavior.

## Result
Completed with focused sweep runner coverage and full local CI passing.

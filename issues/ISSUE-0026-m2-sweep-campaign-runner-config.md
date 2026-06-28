# ISSUE-0026: M2 sweep campaign runner config

## Status
closed

## Scope
module:13

## Priority
P0

## Owner
- Codex role: Module 13 + Module 11
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for runner/report boundary choices

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0026-m2-sweep-campaign-config
- Worktree: ../f700f-wt-issue-0026
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 13 and 11 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018, ISSUE-0025

## Task
Create M2 700F candidate smoke/full sweep campaign configs for existing sweep runner entry points.

## Acceptance criteria
- [x] Smoke and full campaign configs exist.
- [x] Campaign includes SSB standard/narrow, 700D/700E emulated, official 700D/700E skip-capable entries, and 700F-A/B/C profile-only candidates.
- [x] Smoke campaign completes quickly.
- [x] Unknown or unavailable modes are recorded as skips.
- [x] JSON/CSV aggregate outputs are generated.
- [x] Stable result ordering is verified for the same seed.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: `test_sweep_runner` now expects an M2 700F candidate smoke
  campaign helper, expected M2 mode ordering, skip reasons, JSON/CSV artifacts,
  and stable repeated-run ordering.
- Red result: `bash -lc 'cmake --build build --target test_sweep_runner'`
  failed because `f700f::make_m2_700f_candidate_smoke_sweep_config` was missing.

## Result
Completed. Added M2 700F candidate smoke/full sweep campaign helpers,
TOML-friendly campaign configs, campaign spec docs, and sweep runner tests for
mode ordering, skip reasons, aggregate JSON/CSV generation, and stable ordering.

Verification:
- `bash -lc 'cmake --build build --target test_sweep_runner && ctest --test-dir build -R sweep_runner_contract --output-on-failure'`: passed
- `bash ./tools/run_ci_local.sh`: passed on 2026-06-28 with 11/11 CTest tests,
  version check passed, governance check passed

Notes:
- Clock-skew warnings were observed during build and did not block passing tests.
- No push or PR was performed per manager instruction.

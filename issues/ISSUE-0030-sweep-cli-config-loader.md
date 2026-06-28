# ISSUE-0030: sweep CLI and config loader

## Status
open

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
- [ ] CLI help succeeds.
- [ ] Missing config fails clearly.
- [ ] Smoke config generates artifacts.
- [ ] Output directory override works.
- [ ] Run id override works.
- [ ] Unknown/unavailable modes can be skipped.
- [ ] TOML planned schema is documented if the first loader is simpler.
- [ ] Local CI passes.

## Test plan
- Add red tests before implementation.
- `bash ./tools/run_ci_local.sh`

## TDD record
- Pending.

## Result
Pending.

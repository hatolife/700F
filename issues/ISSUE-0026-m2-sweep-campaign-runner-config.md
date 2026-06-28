# ISSUE-0026: M2 sweep campaign runner config

## Status
open

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
- [ ] Smoke and full campaign configs exist.
- [ ] Campaign includes SSB standard/narrow, 700D/700E emulated, official 700D/700E skip-capable entries, and 700F-A/B/C profile-only candidates.
- [ ] Smoke campaign completes quickly.
- [ ] Unknown or unavailable modes are recorded as skips.
- [ ] JSON/CSV aggregate outputs are generated.
- [ ] Stable result ordering is verified for the same seed.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

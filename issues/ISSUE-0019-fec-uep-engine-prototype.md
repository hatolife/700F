# ISSUE-0019: FEC UEP engine prototype

## Status
open

## Scope
module:06

## Priority
P0

## Owner
- Codex role: Module 06 + Module 02
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for coding theory choices

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0019-fec-uep-prototype
- Worktree: ../f700f-wt-issue-0019
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: module 06 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018

## Task
Prototype bounded FEC/UEP primitives for experimental 700F candidate modes.

## Acceptance criteria
- [ ] Prototype API follows ISSUE-0018 architecture.
- [ ] Deterministic tests cover encode/decode or explicit stub behavior.
- [ ] Candidate artifacts are labeled experimental.
- [ ] No official baseline behavior is changed.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

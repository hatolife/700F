# ISSUE-0032: 700F candidate minimal behavior bridge

## Status
planned

## Scope
module:02,06,07,09,11,13

## Priority
P0

## Owner
- Codex role: Module 02 + candidate implementation modules
- Recommended model: gpt-5.5-xhigh

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0032-700f-candidate-minimal-behavior
- Worktree: ../700F-ISSUE-0032
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0029
- ISSUE-0030
- ISSUE-0031

## Task
Connect 700F profile-only candidates to clearly labeled minimal surrogate
behavior without claiming real modem performance.

## Acceptance criteria
- [ ] Results include `surrogate`, `not_real_modem`, and `not_downselect_valid`.
- [ ] Reports do not treat surrogate rows as RF performance.
- [ ] Any interface expansion is documented before implementation.
- [ ] Local CI passes.

## TDD record
- Pending.

## Result
Pending.

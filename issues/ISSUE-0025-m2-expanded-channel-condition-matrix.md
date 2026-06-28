# ISSUE-0025: M2 expanded channel condition matrix

## Status
open

## Scope
module:08

## Priority
P0

## Owner
- Codex role: Module 08 + Module 13
- Recommended model: GPT-5.3-Codex-Spark; gpt-5.5-medium for channel edge cases

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0025-expanded-channel-matrix
- Worktree: ../f700f-wt-issue-0025
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 08 and 13 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018, ISSUE-0010

## Task
Define an M2 channel condition matrix using existing channel model primitives.

## Acceptance criteria
- [ ] Smoke and full channel matrices are documented.
- [ ] Smoke matrix includes identity, AWGN 6 dB, AWGN 0 dB, seed 1.
- [ ] Full matrix includes AWGN SNR -2, 0, 2, 4, 6, 8 dB; frequency offsets 0, 50, 100, 200 Hz; simple fading none, weak, medium; seeds 1, 2, 3.
- [ ] Condition ids are unique.
- [ ] Invalid SNR/frequency offset and empty seed list are rejected.
- [ ] CI smoke subset and full subset are distinguishable.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

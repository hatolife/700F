# ISSUE-0024: M2 scoring model and comparison policy

## Status
open

## Scope
module:12

## Priority
P0

## Owner
- Codex role: Module 12 + Module 13
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for scoring policy tradeoffs

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0024-scoring-model
- Worktree: ../f700f-wt-issue-0024
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 12 and 13 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018

## Task
Define and implement an interim scoring model for comparing SSB, 700D, 700E, and 700F candidate sweep results.

## Acceptance criteria
- [ ] Scoring model documentation exists.
- [ ] Completed run ratio, skipped/failed penalties, dropout, latency, bandwidth, BER/FER availability, official-unavailable handling, audio-only handling, future ASR WER, future STOI/ESTOI, and subjective note slots are represented.
- [ ] 700F profile snapshot can be included in score output.
- [ ] Deterministic tests cover empty results, completed/failed/skipped comparisons, dropout, latency, bandwidth penalty, BER/FER N/A, and profile snapshot.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

# ISSUE-0020: OFDM waveform candidate prototype

## Status
open

## Scope
module:07

## Priority
P0

## Owner
- Codex role: Module 07 + Module 09
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for modem waveform design

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0020-ofdm-waveform-candidate
- Worktree: ../f700f-wt-issue-0020
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: module 07 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018

## Task
Prototype an experimental OFDM waveform candidate path for 700F exploration.

## Acceptance criteria
- [ ] Waveform assumptions are documented.
- [ ] Deterministic contract tests exist.
- [ ] Output can enter the M1 pipeline or has a documented stub boundary.
- [ ] Candidate mode is not labeled as official baseline.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

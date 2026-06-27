# ISSUE-0011: SSB reference emulator

## Status
open

## Scope
module:10

## Priority
P0

## Owner
- Codex role: Module 10 + Module 04
- Recommended model: GPT-5.3-Codex-Spark; gpt-5.5-low for filter design issues

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0011-ssb-reference-emulator
- Worktree: ../f700f-wt-issue-0011
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 10 and 04 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Implement SSB standard 3k and narrow 1.9k reference emulator profiles.

## Affected paths
- modules/10_reference_baselines_ssb_700d_700e/
- modules/04_audio_io_dataset/
- docs/specs/ssb_reference.md

## Dependencies
- ISSUE-0008, ISSUE-0010

## Acceptance criteria
- [ ] Audio bandwidth limit changes per profile.
- [ ] standard 3k and narrow 1.9k are selectable from mode registry.
- [ ] SSB is treated as audio metrics target, not BER target.
- [ ] Received audio can be exported as WAV or raw float.
- [ ] SSB assumptions are documented.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: to be completed by the issue owner before implementation.
- Red command: to be completed by the issue owner.
- Expected failure: to be completed by the issue owner.
- Green command: see Test plan.
- Refactor notes: to be completed by the issue owner.
- TDD exception if any: planning-only issue until implementation begins.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.

## Result
Pending.

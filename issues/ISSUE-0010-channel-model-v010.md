# ISSUE-0010: Channel model v0.1.0

## Status
open

## Scope
module:08

## Priority
P0

## Owner
- Codex role: Module 08
- Recommended model: GPT-5.3-Codex-Spark; gpt-5.5-medium for numeric validation

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0010-channel-model-v0-1
- Worktree: ../f700f-wt-issue-0010
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; module 08 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Implement minimal AWGN, frequency offset, gain fading, and channel chain models.

## Affected paths
- modules/08_hf_channel_simulator/
- docs/specs/channel_models.md

## Dependencies
- ISSUE-0008

## Acceptance criteria
- [ ] AWGN injects noise close to requested SNR.
- [ ] Frequency offset applies requested Hz.
- [ ] Fading is reproducible with seed.
- [ ] Channel chain order can be configured.
- [ ] Unit tests cover boundary conditions, silence, zero-length blocks, and extreme SNR.

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

# ISSUE-0013: 700D 700E parameterized emulator skeleton

## Status
open

## Scope
module:07,10

## Priority
P1

## Owner
- Codex role: Module 07 + Module 10
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for modem/FEC design

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0013-freedv-parameterized-emulator
- Worktree: ../f700f-wt-issue-0013
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 07 and 10 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create parameterized 700D/700E-style emulator skeleton distinct from official baselines.

## Affected paths
- modules/07_ofdm_modem_waveform/
- modules/10_reference_baselines_ssb_700d_700e/
- docs/specs/freedv_emulator.md

## Dependencies
- ISSUE-0008

## Acceptance criteria
- [ ] Emulator-only results are not labeled official baseline.
- [ ] Official-vs-emulator gaps are documented.
- [ ] Descriptors include carrier count, bandwidth, frame duration, CP duration, raw bitrate, and FEC name.
- [ ] Simplified implementation explicitly marks unimplemented features.
- [ ] Boundary supports later high-quality emulator replacement.

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

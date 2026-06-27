# ISSUE-0012: FreeDV 700D 700E official baseline adapter

## Status
open

## Scope
module:05,10

## Priority
P0

## Owner
- Codex role: Module 05 + Module 10
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for FreeDV API/mode definitions

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0012-freedv-official-baseline-adapter
- Worktree: ../f700f-wt-issue-0012
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 05 and 10 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Create optional Codec2/FreeDV adapter for official 700D and 700E baselines.

## Affected paths
- modules/05_codec_adapter/
- modules/10_reference_baselines_ssb_700d_700e/
- docs/specs/freedv_official_baselines.md

## Dependencies
- ISSUE-0007, ISSUE-0008

## Acceptance criteria
- [ ] F700F_ENABLE_CODEC2=OFF still builds.
- [ ] F700F_ENABLE_CODEC2=ON includes codec2 adapter target.
- [ ] 700D and 700E mode descriptors are retrievable.
- [ ] Sample audio can flow through official 700D/700E path.
- [ ] Errors distinguish codec2 not configured, mode unsupported, and runtime failure.

## Test plan
- cmake -S . -B build -DF700F_ENABLE_CODEC2=OFF
- cmake --build build
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

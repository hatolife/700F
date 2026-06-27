# ISSUE-0005: GitHub Actions CI introduction

## Status
open

## Scope
project

## Priority
P0

## Owner
- Codex role: Module 01 / Manager
- Recommended model: GPT-5.3-Codex-Spark; gpt-5.5-low for CI failures; gpt-5.5-medium for Windows

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0005-github-actions-ci
- Worktree: ../f700f-wt-issue-0005
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; module 01 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Ensure GitHub Actions automatically runs configure/build/test/governance checks with Linux required and Windows optional.

## Affected paths
- .github/workflows/ci.yml
- docs/ci.md
- tools/check_submodules.py

## Dependencies
- ISSUE-0004

## Acceptance criteria
- [ ] CMake configure passes.
- [ ] Build passes.
- [ ] CTest passes.
- [ ] Version manifest check passes.
- [ ] Governance check passes.
- [ ] Submodule uninitialized state fails clearly or skip conditions are explicit.
- [ ] Local CI/GitHub CI mapping is documented.

## Test plan
- cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
- cmake --build build
- ctest --test-dir build --output-on-failure
- python3 tools/check_versions.py
- python3 tools/check_governance.py

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

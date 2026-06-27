# ISSUE-0007: external codec2 submodule pin

## Status
open

## Scope
dependency

## Priority
P0

## Owner
- Codex role: Manager + Module 05 + Module 10
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for Codec2/FreeDV internals

## Branch
- Base branch: develop
- Work branch: research/ISSUE-0007-codec2-submodule-pin
- Worktree: ../f700f-wt-issue-0007
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 05 and 10 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
external/codec2 pointer update

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Pin Codec2/FreeDV as external/codec2 submodule and document official 700D/700E baseline sources.

## Affected paths
- .gitmodules
- external/codec2
- docs/dependencies/codec2.md
- docs/codec2_700d_700e_verification.md
- tools/check_submodules.py
- CMakeLists.txt

## Dependencies
- ISSUE-0004

## Acceptance criteria
- [ ] Submodule commit hash is documented.
- [ ] Bootstrap build works with Codec2 disabled.
- [ ] Codec2-enabled build path exists behind F700F_ENABLE_CODEC2.
- [ ] 700D/700E constants or mode info are recorded with pinned source file path, symbol, and commit hash.
- [ ] Web-derived values are not treated as authoritative.

## Test plan
- git submodule status --recursive
- python3 tools/check_submodules.py
- cmake -S . -B build -DF700F_ENABLE_CODEC2=OFF

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

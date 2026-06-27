# ISSUE-0001: Bootstrap repository

## Status
closed

## Scope
project

## Priority
P0

## Owner
- Codex role: Manager
- Model tier: xhigh-architect

## Branch
- Base branch: develop
- Work branch: work
- Worktree: /workspace/700F
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.0.1
- Module version: v0.0.1
- Compatibility line: 0
- Milestone: M0 Bootstrap

## Submodule impact
none

## Background
Initialize the FreeDV 700F exploration repository according to `codex_freedv700f_instruction.md`.

## Task
Create governance docs, version files, module skeletons, minimal C++20 build, and local validation scripts.

## Affected paths
- AGENTS.md
- docs/
- issues/
- modules/
- include/
- src/
- tools/

## Dependencies
- none

## Acceptance criteria
- [x] project/module version files exist
- [x] AGENTS.md files exist
- [x] build/test/governance checks pass
- [x] related README/index files updated

## Test plan
- cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
- cmake --build build
- ctest --test-dir build --output-on-failure
- python3 tools/check_versions.py
- python3 tools/check_governance.py

## TDD record
- TDD exception if any: bootstrap creates the test harness and governance scripts in the same issue.

## Work log
- 2026-06-27 00:00 UTC Created and completed bootstrap skeleton.

## Result
Repository bootstrap completed.

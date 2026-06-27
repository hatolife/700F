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
- [x] CMake configure passes.
- [x] Build passes.
- [x] CTest passes.
- [x] Version manifest check passes.
- [x] Governance check passes.
- [x] Submodule uninitialized state fails clearly or skip conditions are explicit.
- [x] Local CI/GitHub CI mapping is documented.

## Test plan
- cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
- cmake --build build
- ctest --test-dir build --output-on-failure
- python3 tools/check_versions.py
- python3 tools/check_governance.py

## TDD record
- Red test added: no optional Windows matrix and no explicit `F700F_ENABLE_CODEC2=OFF` passed to CMake in CI.
- Red command: `rg -n 'runs-on:\\s*windows|Windows|windows:' .github/workflows/ci.yml | grep -q . || (echo 'Expected failure: Linux-only CI does not include optional Windows path yet.' >&2; exit 1)`  
  `rg -n 'F700F_ENABLE_CODEC2' .github/workflows/ci.yml | grep -q . || (echo 'Expected failure: codec2 toggle not pinned in CI yet.' >&2; exit 1)`
- Expected failure: both commands fail because the workflow currently has a single Linux job and no explicit codec2 off switch.
- Green command: `rg -n 'runs-on:\\s*windows-latest|continue-on-error:\\s*true|F700F_ENABLE_CODEC2=OFF' .github/workflows/ci.yml`  
  `bash ./tools/run_ci_local.sh`
- Refactor notes: no source refactor required; workflow and documentation aligned.
- TDD exception if any: planning-only issue until implementation begins.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-27 14:55 UTC Red TDD evidence captured: `rg` checks confirmed `.github/workflows/ci.yml` lacks optional Windows job and `F700F_ENABLE_CODEC2` override.
- 2026-06-27 15:22 UTC Added Linux-required + optional-Windows GitHub Actions CI job matrix in `.github/workflows/ci.yml` and updated `docs/ci.md` to document local-to-GitHub mapping.
- 2026-06-27 15:24 UTC Ran `bash ./tools/run_ci_local.sh`: pass (configure/build/test/version/governance all passed).
- 2026-06-27 15:24 UTC Green TDD checks pass:
  - `rg` confirms presence of Linux-required/Windows-optional jobs and explicit `F700F_ENABLE_CODEC2=OFF`.
  - Local CI wrapper passes with repository settings currently configured.

## Result
Added `.github/workflows/ci.yml` Linux-required + optional Windows job wiring, added codec2-off defaulting in CMake configure, added explicit submodule initialization guard, and documented CI mapping in `docs/ci.md`. Local CI passes in this worktree.

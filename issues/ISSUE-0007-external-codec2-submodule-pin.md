# ISSUE-0007: external codec2 submodule pin

## Status
closed

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
- PR/Merge status: merged local to develop as `88793a1`; push/PR not performed because GitHub permission is blocked

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
- [x] Submodule commit hash is documented.
- [x] Bootstrap build works with Codec2 disabled.
- [x] Codec2-enabled build path exists behind F700F_ENABLE_CODEC2.
- [x] 700D/700E constants or mode info are recorded with pinned source file path, symbol, and commit hash.
- [x] Web-derived values are not treated as authoritative.

## Test plan
- git submodule status --recursive
- python3 tools/check_submodules.py
- cmake -S . -B build -DF700F_ENABLE_CODEC2=OFF

## TDD record
- Red test added: planned `tools/check_submodules.py` repository dependency check for `external/codec2` metadata/pin.
- Red command: `python3 tools/check_submodules.py`
- Expected failure: missing check script before implementation: `can't open file ... tools/check_submodules.py: [Errno 2] No such file or directory`.
- Green command: `python3 tools/check_submodules.py`; `bash ./tools/run_ci_local.sh`; `bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"`.
- Refactor notes: implementation should keep `F700F_ENABLE_CODEC2=OFF` independent of submodule initialization.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-27 14:40 UTC Subagent D started work in `research/ISSUE-0007-codec2-submodule-pin`; pre-implementation evidence: `git submodule status --recursive` had no output, `python3 tools/check_submodules.py` failed because the script was absent, and a direct PowerShell CMake probe was blocked by local PATH before code changes.
- 2026-06-27 14:55 UTC Added Codec2/FreeDV submodule from `https://github.com/drowe67/codec2.git` at `external/codec2`, pinned to `310777b1c6f1af0bc7c72f5b32f80f6fd9136962` (`1.2.0-108-g310777b`).
- 2026-06-27 15:10 UTC Added `F700F_ENABLE_CODEC2` OFF-by-default CMake gate, `tools/check_submodules.py`, dependency notes, and pinned 700D/700E verification anchors without implementing adapter or mode behavior.
- 2026-06-27 15:20 UTC Verification passed: `python3 tools/check_submodules.py`; `git submodule status --recursive`; `bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"`; `bash ./tools/run_ci_local.sh` with Codec2 disabled by default. CI emitted clock-skew warnings only.
- 2026-06-27 15:25 UTC Pre-commit `git status --short --branch` recorded: branch `research/ISSUE-0007-codec2-submodule-pin`; added `.gitmodules`, `external/codec2`, docs, and `tools/check_submodules.py`; modified `CMakeLists.txt`, root/module ISSUE-0007 files, and `CHANGELOG.md`.
- 2026-06-27 14:33 UTC Manager merged branch `research/ISSUE-0007-codec2-submodule-pin` into `develop` as `88793a1`; post-merge `bash ./tools/run_ci_local.sh` passed.

## Result
Codec2/FreeDV is pinned as `external/codec2` at `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`; verification docs and submodule checks are present. Local CI passes with Codec2 disabled by default, and the branch is merged locally to `develop`.

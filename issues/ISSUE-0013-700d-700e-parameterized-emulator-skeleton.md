# ISSUE-0013: 700D 700E parameterized emulator skeleton

## Status
closed

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
- PR/Merge status: merged local to develop as `89681c7`; push/PR not performed because GitHub permission is blocked

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
- [x] Emulator-only results are not labeled official baseline.
- [x] Official-vs-emulator gaps are documented.
- [x] Descriptors include carrier count, bandwidth, frame duration, CP duration, raw bitrate, and FEC name.
- [x] Simplified implementation explicitly marks unimplemented features.
- [x] Boundary supports later high-quality emulator replacement.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: `tests/test_freedv_emulator.cpp` covers `freedv700d_emulated` and `freedv700e_emulated` descriptors, registry selection, emulator/official flags, extended parameter fields, and unimplemented encode/decode status.
- Red command: `bash -lc 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --target test_freedv_emulator && ctest --test-dir build --output-on-failure -R freedv_emulator_contract'`
- Expected failure: compile failed because `f700f/reference_baselines/freedv_emulator.hpp` did not exist.
- Green command: same focused command passed after implementation.
- Refactor notes: added the minimal descriptor fields needed by ISSUE-0013, a Module 10 emulator factory/runtime, docs for placeholder assumptions, and version metadata for modules 07 and 10.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-27 15:20 UTC M1-B subagent C started ISSUE-0013 in dedicated worktree; recorded TDD plan before adding tests.
- 2026-06-27 15:43 UTC Focused emulator contract passed; local CI passed via `bash ./tools/run_ci_local.sh` with 3 tests.
- 2026-06-27 14:59 UTC Manager merged branch into `develop` as `89681c7`; resolved CMake/version conflicts; post-merge local CI passed.

## Result
Complete. Added descriptor-only FreeDV 700D/700E parameterized emulator skeletons with
explicit non-official emulator flags. Encode/decode return clear ISSUE-0013
not-implemented statuses. Official Codec2/FreeDV baseline work remains out of scope for
ISSUE-0012. Branch is merged locally to `develop`.

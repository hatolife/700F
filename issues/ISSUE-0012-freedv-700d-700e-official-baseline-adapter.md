# ISSUE-0012: FreeDV 700D 700E official baseline adapter

## Status
closed

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
- PR/Merge status: merged local to develop as `5b48e62`

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 05 and 10 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
Uses the existing pinned `external/codec2` submodule when initialized; no submodule pin
change.

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
- [x] F700F_ENABLE_CODEC2=OFF still builds.
- [x] F700F_ENABLE_CODEC2=ON includes codec2 adapter target.
- [x] 700D and 700E mode descriptors are retrievable.
- [ ] Sample audio can flow through official 700D/700E path. Blocked/deferred: v0.1.0
  adapter is a guarded availability skeleton and does not link the Codec2 waveform runtime.
- [x] Errors distinguish codec2 not configured, mode unsupported, and runtime failure.

## Test plan
- cmake -S . -B build -DF700F_ENABLE_CODEC2=OFF
- cmake --build build
- ctest --test-dir build --output-on-failure

## TDD record
- Red test planned before implementation: `test_freedv_official_adapter` will require
  `freedv700d_official` and `freedv700e_official` descriptors, official/emulator flags,
  Codec2-disabled unavailable status text, and guarded encode/decode skip errors.
- Pin-source evidence before implementation: `external/codec2` initialized at
  `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`; `external/codec2/src/freedv_api.h`
  defines `FREEDV_MODE_700D` as `7` and `FREEDV_MODE_700E` as `13`; `external/codec2/src/ofdm_mode.c`
  contains `700D` default mode configuration and a `700E` configuration branch.
- Red command: `bash -lc "rm -rf build && cmake -S . -B build -DF700F_ENABLE_CODEC2=OFF && cmake --build build"`.
- Expected failure: `tests/test_freedv_official_adapter.cpp` fails to compile because
  `f700f/codec_adapter/freedv_official.hpp` does not exist before implementation.
- Green command: `bash ./tools/run_ci_local.sh` passed with 7 CTest tests, version check,
  and governance check. Guarded ON smoke command
  `bash -lc "cmake -S . -B build-codec2 -DF700F_ENABLE_CODEC2=ON && cmake --build build-codec2 --target test_freedv_official_adapter && ./build-codec2/test_freedv_official_adapter"`
  passed before removing generated `build-codec2/`.
- Refactor notes: official descriptor and availability behavior live in Module 05
  `f700f_codec_adapter`; Module 10 emulator descriptors remain unchanged and distinct.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-28 JST M1-C subagent B started branch-first work in
  `../f700f-wt-issue-0012`; initialized Codec2 submodule successfully and recorded
  pin-source evidence before adding tests.
- 2026-06-28 JST Added red test for official descriptors/flags/unavailable behavior,
  implemented guarded Module 05 adapter skeleton, documented official baselines, and
  verified default OFF local CI.
- 2026-06-28 JST Merged to local `develop` as `5b48e62`; CMake and issue index conflicts were resolved by retaining ISSUE-0011 and ISSUE-0012 targets/tests and issue rows. Post-merge local CI passed with 8 CTest tests.

## Result
Implemented and integrated into local `develop` with one explicit limitation: full Codec2 waveform encode/decode and sample-audio round trip validation are deferred to a follow-up issue.

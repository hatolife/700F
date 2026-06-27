# ISSUE-0011: SSB reference emulator

## Status
closed

## Scope
module:10

## Priority
P0

## Owner
- Codex role: Module 10 + Module 04
- Recommended model: GPT-5.3-Codex-Spark; gpt-5.5-low for filter design issues

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0011-ssb-reference-emulator
- Worktree: ../f700f-wt-issue-0011
- Merge target: develop
- PR/Merge status: merged local to develop as `0f09a49`

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 10 and 04 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Implement SSB standard 3k and narrow 1.9k reference emulator profiles.

## Affected paths
- modules/10_reference_baselines_ssb_700d_700e/
- modules/04_audio_io_dataset/
- docs/specs/ssb_reference.md

## Dependencies
- ISSUE-0008, ISSUE-0010

## Acceptance criteria
- [x] Audio bandwidth limit changes per profile.
- [x] standard 3k and narrow 1.9k are selectable from mode registry.
- [x] SSB is treated as audio metrics target, not BER target.
- [ ] Received audio can be exported as WAV or raw float.
- [x] SSB assumptions are documented.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: added in `tests/test_ssb_reference_emulator.cpp` (descriptor retrieval and
  registry-selection failures) before implementation work.
- Red command: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ./build/test_ssb_reference_emulator`.
- Expected failure: before implementation, test expected to fail because `ssb_standard_3k` and
  `ssb_narrow_1k9` descriptors and registration were absent.
- Green command: see Test plan.
- Refactor notes: kept placeholders explicit via `implementation_status` and no BER/FER
  claim in descriptors.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-28 00:00 UTC Started TDD phase in module-local implementation branch before code.
- 2026-06-28 00:00 UTC Implemented `ssb_standard_3k` / `ssb_narrow_1k9` descriptors, registry hooks, placeholder SSB runtime, and added tests.
- 2026-06-28 JST Merged to local `develop` as `0f09a49`; post-merge local CI passed with 7 CTest tests.

## Result
Implemented and integrated into local `develop`. SSB standard 3k and narrow 1.9k modes are selectable and documented as audio-metrics targets. Known limitation for M1 audit: received audio WAV/raw float export remains a follow-up item.

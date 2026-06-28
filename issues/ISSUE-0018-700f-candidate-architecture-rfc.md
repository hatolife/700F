# ISSUE-0018: 700F candidate architecture RFC

## Status
closed

## Scope
architecture

## Priority
P0

## Owner
- Codex role: Module 02 + Manager
- Recommended model: gpt-5.5-xhigh for interface boundaries; gpt-5.5-medium for docs/tests

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0018-700f-candidate-architecture-rfc
- Worktree: ../700F-ISSUE-0018
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 02 and any touched interface modules target v0.2.0
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0017

## Task
Define 700F candidate mode architecture, data contracts, baseline comparison rules, and test expectations before candidate implementation starts.

## Acceptance criteria
- [x] Candidate mode boundary is documented in
  `docs/specs/700f_candidate_architecture.md`.
- [x] Baseline vs candidate labeling rules are documented in
  `docs/specs/700f_candidate_architecture.md`.
- [x] Required updates to `docs/protocol.md` and `docs/module_contracts.md` are made.
- [x] Red-test plan for ISSUE-0019 through ISSUE-0022 is documented in
  `docs/specs/700f_candidate_architecture.md`.
- [x] Three profile-only candidate descriptors and registry helpers are implemented
  in `include/f700f/candidate_profiles.hpp` and `src/candidate_profiles.cpp`.
- [x] Contract tests cover descriptor retrieval, unique ids, 1.9 kHz bandwidth
  targets, audio bandwidth targets, official/emulator flags, profile-only
  encode/decode status, registry selection, metrics snapshots, and sweep/report
  visibility.

## Test plan
- `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`
- `./tools/run_ci_local.sh`

## TDD record
- Red test added: `tests/test_700f_candidate_profiles.cpp` will cover the three
  descriptor-only 700F candidate profiles, registry selection, unimplemented
  encode/decode status, and metrics/sweep/report descriptor snapshot access before
  candidate profile implementation is added.
- Red result: build failed on missing `f700f/candidate_profiles.hpp`.
- Green result: full CTest passed with 10/10 tests after implementation.
- TDD exception if any: none.

## Result
Completed on branch `docs/ISSUE-0018-700f-candidate-architecture-rfc`.

ISSUE-0018 defines the M2-A profile-only 700F candidate boundary and adds selectable
candidate descriptors for:

- `freedv700f_a_balanced`
- `freedv700f_b_robust`
- `freedv700f_c_quality`

The profiles are not official baselines and not emulators. They return clear
`profile_only` encode/decode failures until ISSUE-0019 through ISSUE-0022 implement the
candidate components.

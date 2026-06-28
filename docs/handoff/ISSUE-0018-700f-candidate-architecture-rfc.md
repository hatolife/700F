# ISSUE-0018 700F Candidate Architecture RFC Handoff

## Branch

- Base branch: local `develop`
- Work branch: `docs/ISSUE-0018-700f-candidate-architecture-rfc`
- Worktree: `../700F-ISSUE-0018`
- Merge target: `develop`
- Push/PR: not performed by request

## Completed

- Added the M2-A 700F candidate architecture boundary in
  `docs/specs/700f_candidate_architecture.md`.
- Added the candidate profile catalog in `docs/specs/700f_candidate_profiles.md`.
- Updated `docs/protocol.md`, `docs/module_contracts.md`, and
  `docs/specs/mode_interface.md` with the profile-only contract.
- Added `include/f700f/candidate_profiles.hpp` and `src/candidate_profiles.cpp`.
- Added profile descriptors and registry/factory helpers for:
  - `freedv700f_a_balanced`
  - `freedv700f_b_robust`
  - `freedv700f_c_quality`
- Added `tests/test_700f_candidate_profiles.cpp`.
- Bumped project/protocol metadata to v0.2.0 and Module 02 metadata to v0.2.0.

## Verification

- Red test: `cmake --build build --target test_700f_candidate_profiles` failed before
  implementation on missing `f700f/candidate_profiles.hpp`.
- Green test: `ctest --test-dir build --output-on-failure` passed 10/10 tests after
  implementation.
- Final local CI: see commit/final result for `./tools/run_ci_local.sh` status.

## Follow-Up Work

- ISSUE-0019 owns concrete FEC/UEP policy implementations for the candidate FEC ids.
- ISSUE-0020 owns OFDM waveform implementation and measured occupied-bandwidth tests.
- ISSUE-0021 owns sync/impairment estimator behavior.
- ISSUE-0022 owns sweep triage/report ranking over completed, skipped, and failed
  candidate records.

## Limitations

The three 700F candidate profiles are descriptor-only stubs. They are selectable through
`ModeRegistry`, but encode/decode return `ok == false` with `profile_only` errors.

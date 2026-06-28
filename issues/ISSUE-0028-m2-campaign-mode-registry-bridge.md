# ISSUE-0028: M2-C campaign mode registry bridge

## Status
closed

## Scope
module:13,10

## Priority
P0

## Owner
- Codex role: Module 13 + Module 10 + Module 02
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for interface boundary choices

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0028-m2-campaign-mode-registry
- Worktree: ../f700f-wt-issue-0028
- Merge target: develop
- PR/Merge status: local branch complete; pending local merge to develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 13 and 10 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018, ISSUE-0026

## Background
M2-B smoke produced JSON/CSV artifacts, but every campaign row was skipped because the M2 campaign runner did not register SSB, 700D/700E emulator, official FreeDV, or 700F candidate profile factories in the smoke runner.

## Task
Connect M2 campaign mode descriptors to registered mode factories or explicit profile-only factories so the smoke campaign can distinguish implemented, unavailable, and profile-only candidate behavior without claiming final 700F modem functionality.

## Acceptance criteria
- [x] M2 smoke runner can register profile-only 700F candidate factories.
- [x] M2 smoke runner can register available SSB and parameterized emulator factories when in default CI scope.
- [x] Official FreeDV modes remain skip-capable when Codec2 is disabled/uninitialized.
- [x] Smoke output includes at least one non-skipped campaign row, or the issue documents why that is not yet possible.
- [x] No official FreeDV waveform parity claim is made.
- [x] Default `F700F_ENABLE_CODEC2=OFF` CI remains green.

## Test plan
- `bash -lc "cmake --build build --target test_sweep_runner && ./build/test_sweep_runner"`
- `bash ./tools/run_ci_local.sh`

## TDD record
- Red test added first in `tests/test_sweep_runner.cpp` for
  `register_m2_campaign_mode_factories()` before the API existed. Build failed
  with `register_m2_campaign_mode_factories is not a member of f700f`.
- Green implementation added the M2 campaign bridge, direct FreeDV emulator
  factory helpers, metadata-only completed records, and explicit official
  FreeDV skip reasons.

## Result
- M2 smoke bridge now registers:
  - `ssb_standard_3k`
  - `ssb_narrow_1k9`
  - `freedv700d_emulated`
  - `freedv700e_emulated`
  - `freedv700f_a_balanced`
  - `freedv700f_b_robust`
  - `freedv700f_c_quality`
- Default smoke keeps `freedv700d_official` and `freedv700e_official` skipped
  with `official_waveform_roundtrip_not_implemented`.
- Test smoke artifact contains 21 completed rows and 9 skipped rows when the
  unknown-mode skip probe is included. Completed rows include 9
  `profile_only_completed` rows and 6 `descriptor_only_completed` rows.
- These rows are metadata/profile-only completions, not waveform performance or
  candidate downselect evidence.
- Remote publication was not performed from this branch.

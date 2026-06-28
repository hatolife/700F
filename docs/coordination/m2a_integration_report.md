# M2-A Integration Report

## Summary

- Integration branch: `develop`
- Integration time: 2026-06-28 21:40:54 +09:00
- Base before M2-A merge: `4622758` (`Merge ISSUE-0017 M2 planning`)
- Integrated issue: ISSUE-0018
- Final M2-A merge head before manager report commit: `59629d4` (`Merge ISSUE-0018 700F candidate architecture RFC`)
- Push/PR: not performed. `origin` is configured, but publication from this workspace remains blocked by GitHub permission.
- Final local CI after ISSUE-0018 merge: passed with 10 CTest tests.

## Integrated Changes

- Added profile-only 700F candidate architecture and profile documentation.
- Added selectable 700F-A/B/C candidate descriptors:
  - `freedv700f_a_balanced`
  - `freedv700f_b_robust`
  - `freedv700f_c_quality`
- Added candidate profile API and tests.
- Recorded that encode/decode remains intentionally unimplemented with clear profile-only status.
- Updated protocol/module contract documentation for candidate profile snapshots.

## Manager Dispatch Conflict

A later manager request asked to dispatch ISSUE-0019 through ISSUE-0022 as scoring/channel/sweep/report work. Existing repository issue files already define those issue numbers as:

- ISSUE-0019: FEC UEP engine prototype
- ISSUE-0020: OFDM waveform candidate prototype
- ISSUE-0021: sync and impairment estimator prototype
- ISSUE-0022: AI-assisted sweep triage

Because existing issue files are the source of truth, the manager did not overwrite or repurpose those issues. The requested scoring/channel/sweep/report work has been split into follow-up issues ISSUE-0024 through ISSUE-0027.

## Verification

- `bash ./tools/run_ci_local.sh`: pass
- CTest: 10 passed, 0 failed
- Version check: pass
- Governance check: pass
- Clock-skew warnings: observed; not a blocker because build/tests passed

## M2-A Smoke And Comparison

No M2-A sweep smoke or comparison report was run in this pass because only ISSUE-0018 was integrated. Sweep/scoring/report implementation is now tracked by ISSUE-0024 through ISSUE-0027.

## Next Startable Issues

- ISSUE-0019 through ISSUE-0022 remain startable according to existing M2 plan, if the project wants to continue FEC/OFDM/sync/AI triage work.
- ISSUE-0024 and ISSUE-0025 are startable for the newly requested scoring/channel matrix work.
- ISSUE-0026 waits for ISSUE-0025.
- ISSUE-0027 waits for ISSUE-0024 and ISSUE-0026.
- ISSUE-0023 remains held.

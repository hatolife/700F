# ISSUE-0028: M2-C campaign mode registry bridge

## Status
open

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
- PR/Merge status: none

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
- [ ] M2 smoke runner can register profile-only 700F candidate factories.
- [ ] M2 smoke runner can register available SSB and parameterized emulator factories when in default CI scope.
- [ ] Official FreeDV modes remain skip-capable when Codec2 is disabled/uninitialized.
- [ ] Smoke output includes at least one non-skipped campaign row, or the issue documents why that is not yet possible.
- [ ] No official FreeDV waveform parity claim is made.
- [ ] Default `F700F_ENABLE_CODEC2=OFF` CI remains green.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

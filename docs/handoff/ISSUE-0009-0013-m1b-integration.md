# ISSUE-0009-0014 M1-B Integration Handoff

## Current State

- Current branch: `develop`
- Latest integrated M1-B merge before handoff commit: `d0f7cd1`
- Completed issues: ISSUE-0009, ISSUE-0010, ISSUE-0013, ISSUE-0014
- Local CI: `bash ./tools/run_ci_local.sh` passed after every merge and after the final ISSUE-0009 merge.
- Remote: `origin` is configured as `git@github.com:hatolife/700F.git`
- Push/PR: not performed; previous push was blocked by GitHub permission.

## Integrated Branches

| Issue | Branch | Commit | Merge commit | Result |
|---|---|---|---|---|
| ISSUE-0014 | `feature/ISSUE-0014-metrics-artifact-format` | `f694df4` | `b518264` | merged |
| ISSUE-0010 | `feature/ISSUE-0010-channel-model-v0-1` | `eb45469` | `eb12db2` | merged |
| ISSUE-0013 | `feature/ISSUE-0013-freedv-parameterized-emulator` | `2e0973c` | `89681c7` | merged |
| ISSUE-0009 | `feature/ISSUE-0009-deterministic-simulation-core` | `82154f8` | `d0f7cd1` | merged |

## Conflict Notes

- ISSUE-0010 CMake conflict resolved by retaining both metrics and channel targets/tests.
- ISSUE-0013 CMake/version conflicts resolved by retaining metrics, channel, and emulator targets/tests and setting modules 07/08/10/12 to `v0.1.0`.
- ISSUE-0009 CMake/changelog conflicts resolved by retaining all M1-B targets/tests and adding the simulation pipeline test; module 11 version set to `v0.1.0`.

## Next Work

- Start ISSUE-0011 SSB reference emulator.
- Start ISSUE-0012 FreeDV 700D/700E official baseline adapter after running `git submodule update --init --recursive` in the worker worktree.
- Start ISSUE-0015 sweep runner.
- Keep ISSUE-0016 for the final M1 audit after remaining M1 work is complete.

## Known Blockers

- Remote publication is blocked until a user with GitHub push permission publishes `develop`.
- The local `develop` upstream still points to missing `origin/main`; this is remote tracking hygiene, not a local integration blocker.
- Codec2 remains optional for normal CI; official adapter workers must initialize the submodule explicitly.

## Verification

- Final local CI: pass
- Final CTest count: 6 passed
- Version check: pass
- Governance check: pass

# ISSUE-0005-0008 M1-A Integration Handoff

## Current State

- Current branch: `develop`
- Latest integrated M1-A merge before handoff commit: `88793a1`
- Completed issues: ISSUE-0005, ISSUE-0006, ISSUE-0007, ISSUE-0008
- Local CI: `bash ./tools/run_ci_local.sh` passed after every merge and after the final ISSUE-0007 merge.
- Git status before handoff/report commit: clean on `develop`
- Remote: `origin` is configured as `git@github.com:hatolife/700F.git`
- Push/PR: not performed; previous push was blocked by GitHub permission.

## Integrated Branches

| Issue | Branch | Commit | Merge commit | Result |
|---|---|---|---|---|
| ISSUE-0006 | `docs/ISSUE-0006-governance-structure` | `c6449b2` | `9a118cc` | merged |
| ISSUE-0005 | `docs/ISSUE-0005-github-actions-ci` | `4dc7606` | `daeffff` | merged |
| ISSUE-0008 | `feature/ISSUE-0008-mode-interface-v0-1` | `077d7c7` | `67c6327` | merged |
| ISSUE-0007 | `research/ISSUE-0007-codec2-submodule-pin` | `6ab5b0a` | `88793a1` | merged |

## Next Work

- Start ISSUE-0009 deterministic simulation core.
- Start ISSUE-0010 channel model.
- Start ISSUE-0013 700D/700E parameterized emulator skeleton.
- Start ISSUE-0012 FreeDV 700D/700E official baseline adapter after confirming submodule availability in the worker environment.
- Keep ISSUE-0011 SSB reference emulator held until ISSUE-0010 is complete.

## Known Blockers

- Remote publication is blocked until a user with GitHub push permission publishes `develop`.
- The local `develop` upstream still points to missing `origin/main`; this is a remote tracking hygiene issue, not a local integration blocker.
- CI emits occasional clock-skew warnings in WSL builds; tests still pass.

## Worker Notes

- Future workers should branch from local `develop`.
- Normal bootstrap CI should keep `F700F_ENABLE_CODEC2=OFF`.
- Codec2-specific work must use the pinned submodule and avoid treating web-derived values as authoritative.

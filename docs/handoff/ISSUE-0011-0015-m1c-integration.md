# ISSUE-0011-0015 M1-C Integration Handoff

## Current State

- Current branch: `develop`
- Latest integrated M1-C merge before handoff commit: `1710785`
- Completed issues: ISSUE-0011, ISSUE-0012, ISSUE-0015
- Local CI: `bash ./tools/run_ci_local.sh` passed after every merge and after the final ISSUE-0015 merge.
- Remote: `origin` is configured as `git@github.com:hatolife/700F.git`
- Push/PR: not performed; previous push was blocked by GitHub permission.

## Integrated Branches

| Issue | Branch | Commit | Merge commit | Result |
|---|---|---|---|---|
| ISSUE-0011 | `feature/ISSUE-0011-ssb-reference-emulator` | `3f38149` | `0f09a49` | merged |
| ISSUE-0012 | `feature/ISSUE-0012-freedv-official-baseline-adapter` | `ca928ef` | `5b48e62` | merged |
| ISSUE-0015 | `feature/ISSUE-0015-sweep-runner-v0-1` | `6b2e591` | `1710785` | merged |

## Conflict Notes

- ISSUE-0012 CMake conflict resolved by retaining both SSB and official FreeDV adapter targets/tests.
- ISSUE-0012 issue index conflict resolved by retaining both ISSUE-0011 and ISSUE-0012 rows.
- ISSUE-0015 CMake/changelog conflicts resolved by retaining all M1-C targets/tests and changelog entries.

## Known Limitations

- ISSUE-0011 received-audio WAV/raw float export remains a follow-up.
- ISSUE-0012 full Codec2 waveform encode/decode and sample-audio round trip validation remain a follow-up.
- ISSUE-0015 TOML parsing and standalone CLI remain a follow-up.

## Next Work

- Run and record the M1 baseline smoke sweep result.
- Start ISSUE-0016 M1 audit from local `develop`.
- Start M2 planning only if the M1 audit records GO or GO_WITH_LIMITATIONS.

## Verification

- Final local CI: pass
- Final CTest count: 9 passed
- Version check: pass
- Governance check: pass

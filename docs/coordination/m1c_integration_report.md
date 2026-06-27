# M1-C Integration Report

## Summary

- Integration branch: `develop`
- Integration time: 2026-06-28 00:37:10 +09:00
- Base before M1-C merges: `23c4525` (`ISSUE-0009-0014 record M1-B integration handoff`)
- Final M1-C merge head before manager report commit: `1710785` (`Merge ISSUE-0015 sweep runner`)
- Push/PR: not performed. `origin` is configured, but previous push was blocked by GitHub permission.
- Final local CI: passed with 9 CTest tests.

## Subagents

| Issue | Agent | Branch | Worktree | Commit | Local CI | Merge commit |
|---|---|---|---|---|---|---|
| ISSUE-0011 | Arendt | `feature/ISSUE-0011-ssb-reference-emulator` | `../f700f-wt-issue-0011` | `3f38149` | pass | `0f09a49` |
| ISSUE-0012 | Goodall | `feature/ISSUE-0012-freedv-official-baseline-adapter` | `../f700f-wt-issue-0012` | `ca928ef` | pass | `5b48e62` |
| ISSUE-0015 | Wegener | `feature/ISSUE-0015-sweep-runner-v0-1` | `../f700f-wt-issue-0015` | `6b2e591` | pass | `1710785` |

## Merge Order And CI

1. ISSUE-0011 SSB reference emulator
   - Command: `git merge --no-ff feature/ISSUE-0011-ssb-reference-emulator`
   - Result: pass, no conflict
   - Post-merge CI: `bash ./tools/run_ci_local.sh` passed with 7 tests
2. ISSUE-0012 FreeDV official baseline adapter
   - Command: `git merge --no-ff feature/ISSUE-0012-freedv-official-baseline-adapter`
   - Result: CMake and issue index conflicts resolved by keeping both ISSUE-0011 and ISSUE-0012 targets/tests and issue rows
   - Post-merge CI: `bash ./tools/run_ci_local.sh` passed with 8 tests
3. ISSUE-0015 sweep runner
   - Command: `git merge --no-ff feature/ISSUE-0015-sweep-runner-v0-1`
   - Result: CMake and changelog conflicts resolved by keeping all M1-C test targets and changelog entries
   - Post-merge CI: `bash ./tools/run_ci_local.sh` passed with 9 tests

## Integrated Changes

- ISSUE-0011 added selectable `ssb_standard_3k` and `ssb_narrow_1k9` reference modes and documented SSB as an audio-metrics baseline.
- ISSUE-0012 added guarded official FreeDV 700D/700E descriptors and Codec2 availability/error reporting while keeping default `F700F_ENABLE_CODEC2=OFF` builds green.
- ISSUE-0015 added the initial sweep runner, short smoke config schema, stable JSON/CSV aggregate output, skip records, and failed-run continuation.

## Limitations For M1 Audit

- ISSUE-0011 WAV/raw float received-audio export remains deferred.
- ISSUE-0012 full Codec2 waveform encode/decode and sample-audio round trip validation remain deferred.
- ISSUE-0015 TOML parsing and standalone CLI are not implemented; the smoke path is currently exercised by the C++ contract test helper.
- No 700F candidate algorithm implementation was started.

## Next Issue Unlock

- ISSUE-0016 M1 audit is startable.
- M2 planning can start only after the M1 audit records a GO or GO_WITH_LIMITATIONS decision.

## Final Verification

- Final command: `bash ./tools/run_ci_local.sh`
- Result: pass
- Final CTest count: 9 passed, 0 failed
- Version check: pass
- Governance check: pass

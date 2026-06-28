# ISSUE-0029: SSB audio export

## Status
open

## Scope
module:10,11,15

## Priority
P0

## Owner
- Codex role: Module 10 + Module 11 + Module 15
- Recommended model: GPT-5.3-Codex-Spark; fallback gpt-5.5-low

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0029-ssb-audio-export
- Worktree: ../700F-ISSUE-0029
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0023
- ISSUE-0028

## Task
Save SSB reference pipeline output audio as an artifact and expose the artifact
path to reports without changing default Codec2 behavior.

## Acceptance criteria
- [ ] SSB standard/narrow output audio artifact path is recorded when export is enabled.
- [ ] M2 smoke SSB rows can expose an audio artifact path.
- [ ] Export disabled records clear N/A.
- [ ] Zero-length audio does not crash.
- [ ] Invalid output directory fails with a clear error.
- [ ] Local CI passes.

## Test plan
- Add red tests before implementation.
- `bash ./tools/run_ci_local.sh`

## TDD record
- Pending.

## Result
Pending.

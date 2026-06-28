# ISSUE-0031: report JSON loader and CLI

## Status
open

## Scope
module:15,12

## Priority
P0

## Owner
- Codex role: Module 15 + Module 12
- Recommended model: gpt-5.5-medium; fallback gpt-5.5-xhigh

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0031-report-json-loader-cli
- Worktree: ../700F-ISSUE-0031
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0023
- ISSUE-0027
- ISSUE-0028

## Task
Load sweep artifact JSON/CSV and generate a comparison Markdown report through a
CLI.

## Acceptance criteria
- [ ] CLI help succeeds.
- [ ] Missing input fails clearly.
- [ ] Sample JSON generates Markdown.
- [ ] Skipped rows appear in the report.
- [ ] Profile-only rows appear in the report.
- [ ] Descriptor-only rows appear in the report.
- [ ] Report states whether real downselect is possible.
- [ ] Malformed JSON/CSV fails clearly.
- [ ] Local CI passes.

## Test plan
- Add red tests before implementation.
- `bash ./tools/run_ci_local.sh`

## TDD record
- Pending.

## Result
Pending.

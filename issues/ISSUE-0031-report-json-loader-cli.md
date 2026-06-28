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
- [x] CLI help succeeds.
- [x] Missing input fails clearly.
- [x] Sample JSON generates Markdown.
- [x] Skipped rows appear in the report.
- [x] Profile-only rows appear in the report.
- [x] Descriptor-only rows appear in the report.
- [x] Report states whether real downselect is possible.
- [x] Malformed JSON/CSV fails clearly.
- [ ] Local CI passes.

## Test plan
- Add red tests before implementation.
- `bash ./tools/run_ci_local.sh`

## TDD record
- 2026-06-29 JST: Started on
  `feature/ISSUE-0031-report-json-loader-cli` from local `develop`.
  Planning red tests for aggregate JSON/CSV loading, CLI help/error behavior,
  Markdown generation, and downselect feasibility labeling before implementation.
- 2026-06-29 JST: Red-test attempt added report loader/CLI tests before
  implementation; initial PowerShell configure failed because `cmake` was not on
  PATH, then Bash local CI built the new tests after implementation.
- 2026-06-29 JST: Report-specific checks pass:
  `ctest --test-dir build -R 'f700f_report|report_artifact' --output-on-failure`.
  Full `bash ./tools/run_ci_local.sh` is blocked by unrelated
  `sweep_config_loader_contract` and `sweep_cli_missing_config` failures in the
  ISSUE-0030 sweep CLI/config-loader area.

## Result
Implemented JSON/CSV report loader, `f700f-report` CLI, report/CLI tests, and docs.

# ISSUE-0027: M2 baseline comparison report generation

## Status
complete

## Scope
module:15

## Priority
P0

## Owner
- Codex role: Module 15 + Module 12
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for report/downselect policy

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0027-baseline-comparison-report
- Worktree: ../f700f-wt-issue-0027
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 15 and 12 target v0.2.0 if implementation lands
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018, ISSUE-0024, ISSUE-0026

## Task
Generate Markdown baseline comparison reports from M2 sweep/scoring artifacts.

## Acceptance criteria
- [x] Report spec exists.
- [x] Report includes run id, commit hash, config path, mode list, channel list, seeds, skipped/unavailable modes, score table, raw metrics summary, known limitations, official FreeDV status, SSB reference status, 700F candidate summary, and next recommendation placeholder.
- [x] Empty or minimal result inputs are handled deterministically.
- [x] Skipped modes and 700F candidate profiles are displayed.
- [x] Timestamped output filename is supported.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: `tests/test_baseline_comparison_report.cpp` was added before
  implementation and initially failed at configure because the Module 15 report
  generator source/header did not exist.
- Green test: `baseline_comparison_report_contract` now passes and covers
  deterministic Markdown output for empty/minimal score inputs, skipped official
  baselines, profile-only 700F candidates, unavailable official FreeDV status,
  timestamped filenames, and blocked/missing sweep execution context.

## Result
Implemented Module 15 M2 baseline comparison Markdown report generation from ISSUE-0024
scoring output. Added report context metadata, deterministic rendering, skipped and
official-unavailable mode summaries, profile-only 700F candidate display, raw metric
summary tables, known limitations, next recommendation placeholder, and timestamped
filename support.

Local CI: `bash ./tools/run_ci_local.sh` passed on 2026-06-28 with 12/12 CTest tests,
version check passed, and governance check passed. Build emitted clock-skew warnings but
completed successfully.

Remaining limitation: ISSUE-0027 does not execute sweeps or parse sweep artifacts from
disk; missing or blocked sweep execution is represented through report context
`sweep_status` until ISSUE-0026 provides merged sweep outputs.

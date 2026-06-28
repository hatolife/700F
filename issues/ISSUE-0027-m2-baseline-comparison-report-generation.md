# ISSUE-0027: M2 baseline comparison report generation

## Status
open

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
- [ ] Report spec exists.
- [ ] Report includes run id, commit hash, config path, mode list, channel list, seeds, skipped/unavailable modes, score table, raw metrics summary, known limitations, official FreeDV status, SSB reference status, 700F candidate summary, and next recommendation placeholder.
- [ ] Empty or minimal result inputs are handled deterministically.
- [ ] Skipped modes and 700F candidate profiles are displayed.
- [ ] Timestamped output filename is supported.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.

## Result
Pending.

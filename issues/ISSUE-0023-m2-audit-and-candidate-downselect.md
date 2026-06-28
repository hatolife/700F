# ISSUE-0023: M2 audit and candidate downselect

## Status
closed

## Scope
qa

## Priority
P0

## Owner
- Codex role: Manager + QA
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for downselect decisions

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0023-m2-audit-downselect
- Worktree: ../f700f-wt-issue-0023
- Merge target: develop
- PR/Merge status: local branch complete; pending local merge to develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: module 16 target v0.2.0 if QA assets are added
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0018 through ISSUE-0022

## Task
Audit M2 outputs and decide whether a candidate is ready for M3 refinement.

## Acceptance criteria
- [x] Candidate artifacts are listed.
- [x] Baseline comparison limitations are listed.
- [x] Security, dependency, and submodule risks are documented.
- [x] Decision records continue, pivot, or stop recommendation.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- TDD exception: audit/report-only issue unless tooling changes are needed.

## Result
Decision: `M2_GO_WITH_LIMITATIONS`.

M2-C smoke is no longer all skipped, but real candidate downselect is still
prohibited. Completed rows are infrastructure readiness evidence only:

- 6 SSB pipeline rows.
- 6 FreeDV descriptor-only emulator rows.
- 9 700F profile-only candidate rows.

Blocked before real downselect:

- 700F candidates are profile-only.
- 700D/700E emulator rows are descriptor-only.
- Official FreeDV waveform roundtrip is incomplete.
- SSB audio export is incomplete.
- Report generator has no JSON loader/CLI.

Reports:

- `reports/20260628_235652_m2_limited_audit.md`
- `modules/16_golden_benchmark_integration_qa/reports/20260628_235652_m2_limited_audit.md`
- `docs/coordination/m2_limited_audit_report.md`
- `docs/handoff/ISSUE-0023-limited-m2-audit.md`

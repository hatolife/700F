# ISSUE-0002: Fast push CI policy and workflows

## Status
closed

## Scope
project

## Priority
P0

## Owner
- Codex role: Manager
- Model tier: low-fixer

## Branch
- Base branch: work
- Work branch: work
- Worktree: /workspace/700F
- Merge target: work
- PR/Merge status: none

## Target version and milestone
- Project version: v0.0.1
- Module version: n/a
- Compatibility line: 0
- Milestone: M0 Bootstrap

## Submodule impact
none

## Background
User requested that repository instructions avoid GitHub PR usage, encourage frequent pushes, and prioritize early CI execution without blocking all other work while CI is running.

## Task
Update manager instructions and add initial GitHub Actions workflows so pushes exercise CI quickly.

## Affected paths
- AGENTS.md
- docs/ci.md
- issues/README.md
- .github/workflows/ci.yml
- .github/workflows/governance.yml
- .github/workflows/security.yml

## Dependencies
- ISSUE-0001

## Acceptance criteria
- [x] AGENTS.md says GitHub PRs are not required for normal work.
- [x] AGENTS.md encourages frequent pushes and CI-driven iteration.
- [x] CI workflow runs local build/test/governance checks on push and manual dispatch.
- [x] Governance and security placeholder workflows exist early.
- [x] local checks pass.

## Test plan
- ./tools/run_ci_local.sh
- python3 tools/check_governance.py

## TDD record
- TDD exception if any: workflow policy/configuration change; verified through local governance checks and YAML readability.

## Work log
- 2026-06-27 00:00 UTC Created and completed.

## Result
Fast push CI policy and initial workflows added.

# ISSUE-0035: clock-skew warning investigation

## Status
in_progress

## Scope
module:01

## Branch
- Work branch: chore/ISSUE-0035-clock-skew-warning-investigation

## Task
Investigate local build clock-skew warnings without risky build-system churn.

## Acceptance criteria
- [x] Warning source is documented.
- [x] Build and tests remain green.

## TDD record
- TDD exception unless code changes are made.

## Result
- Source investigation completed in this issue worktree.
- Warnings are linked to `/mnt/c` 9p-mounted build metadata timing and remain
  non-blocking in local CI.

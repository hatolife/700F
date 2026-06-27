# ISSUE-0018: 700F candidate architecture RFC

## Status
open

## Scope
architecture

## Priority
P0

## Owner
- Codex role: Module 02 + Manager
- Recommended model: gpt-5.5-xhigh for interface boundaries; gpt-5.5-medium for docs/tests

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0018-700f-candidate-architecture-rfc
- Worktree: ../f700f-wt-issue-0018
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.2.0 target
- Module version: modules 02 and any touched interface modules target v0.2.0
- Compatibility line: 2
- Milestone: M2 700F Candidate Exploration

## Dependencies
- ISSUE-0017

## Task
Define 700F candidate mode architecture, data contracts, baseline comparison rules, and test expectations before candidate implementation starts.

## Acceptance criteria
- [ ] Candidate mode boundary is documented.
- [ ] Baseline vs candidate labeling rules are documented.
- [ ] Required updates to `docs/protocol.md` and `docs/module_contracts.md` are identified or made.
- [ ] Red-test plan for ISSUE-0019 through ISSUE-0022 is documented.

## Test plan
- ./tools/run_ci_local.sh

## TDD record
- Red test added: to be completed by issue owner before implementation.
- TDD exception if any: RFC-only work may document tests without code changes.

## Result
Pending.

# ISSUE-0004 handoff: M0 audit to M1-A entry

## Current state
- ISSUE-0003 planning is integrated locally into `develop` at `eba941a`.
- ISSUE-0004 M0 audit is complete on `docs/ISSUE-0004-m0-audit`.
- Remote is configured: `git@github.com:hatolife/700F.git`.
- Remote `develop` does not exist yet because push was blocked by GitHub permissions.

## M1-A entry
- M1-A may begin locally with ISSUE-0005 through ISSUE-0008 from local `develop`.
- Remote M1-A parallel work must wait until `develop` is pushed by an account with permission.
- Each worker must use the branch recorded in its issue file.
- Each worker must branch from `develop` and target `develop`.
- Each worker must record TDD-first evidence in its issue before implementation.

## Hold line
- Do not start ISSUE-0009 or later deterministic simulation core work yet.
- Do not start channel model, SSB, 700D, or 700E implementation yet.
- Do not use `master` as a base, target, or day-to-day integration branch.

## Push and PR status
- Local commits are ready.
- Push status: blocked; GitHub denied permission to user `xcd0`.
- PR status: not created because push failed.
- `Remote not configured`: not applicable after origin setup.

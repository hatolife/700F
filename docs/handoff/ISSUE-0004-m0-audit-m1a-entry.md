# ISSUE-0004 handoff: M0 audit to M1-A entry

## Current state
- ISSUE-0003 planning is integrated locally into `develop` at `eba941a`.
- ISSUE-0004 M0 audit is complete on `docs/ISSUE-0004-m0-audit`.
- Current branch: `docs/ISSUE-0004-m0-audit`
- Latest commit: branch HEAD final checkpoint (`ISSUE-0004 audit M0 bootstrap and M1 readiness`)
- Local CI result: `bash ./tools/run_ci_local.sh` passed on 2026-06-27 23:08 +09:00.
- Remote is configured: `git@github.com:hatolife/700F.git`.
- Remote `develop` does not exist yet because push was blocked by GitHub permissions.
- `Remote not configured`: not applicable in this workspace.

## M1-A entry
- M1-A may begin locally with ISSUE-0005 through ISSUE-0008 from local `develop`.
- Remote M1-A parallel work must wait until `develop` is pushed by an account with permission.
- Each worker must use the branch recorded in its issue file.
- Each worker must branch from `develop` and target `develop`.
- Each worker must record TDD-first evidence in its issue before implementation.
- Dispatch plan: `docs/coordination/m1a_dispatch_plan.md`

## Next issues
- ISSUE-0005: GPT-5.3-Codex-Spark for initial CI implementation; gpt-5.5-low for CI failure investigation; gpt-5.5-medium for Windows CI issues.
- ISSUE-0006: GPT-5.3-Codex-Spark.
- ISSUE-0008: gpt-5.5-medium; gpt-5.5-xhigh if interface boundary decisions block progress.
- ISSUE-0007: gpt-5.5-medium; gpt-5.5-xhigh if Codec2/FreeDV internals block progress.

## Hold line
- Do not start ISSUE-0009 or later deterministic simulation core work yet.
- Do not start channel model, SSB, 700D, or 700E implementation yet.
- ISSUE-0010 channel model may begin only after ISSUE-0008 lands the minimal protocol needed by channel contracts.
- Do not use `master` as a base, target, or day-to-day integration branch.

## Push and PR status
- Local commits are ready.
- Push status: blocked previously by GitHub permission for user `xcd0`; not retried in this final audit pass.
- PR status: not created; PRs are not required for normal manager work unless requested by a human.
- `Remote not configured`: not applicable after origin setup.

## Known blockers
- Remote publication is blocked until a user with GitHub push permission publishes `develop` and active issue branches.
- PowerShell PATH does not include CMake; WSL `bash ./tools/run_ci_local.sh` passes.
- ISSUE-0009 and later implementation are blocked on ISSUE-0008.

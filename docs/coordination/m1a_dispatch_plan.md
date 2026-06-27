# M1-A Dispatch Plan

## Status

- Source branch for dispatch: local `develop`
- Prepared by: ISSUE-0004 M0 audit
- M1-A state: ready for local parallel work after the ISSUE-0004 final checkpoint commit is merged to `develop`
- Remote state: `origin` is configured, but previous push was blocked by GitHub permission. Remote `develop` is not published.
- PR requirement: none for normal manager work unless a human explicitly asks.

## Dispatch Table

| Issue | Recommended model | Branch | Worktree | Owner module | Prerequisite |
|---|---|---|---|---|---|
| ISSUE-0005 | Initial implementation: GPT-5.3-Codex-Spark; CI failure investigation: gpt-5.5-low; Windows CI issues: gpt-5.5-medium | `docs/ISSUE-0005-github-actions-ci` | `../f700f-wt-issue-0005` | Module 01 / Manager | ISSUE-0004 |
| ISSUE-0006 | GPT-5.3-Codex-Spark | `docs/ISSUE-0006-governance-structure` | `../f700f-wt-issue-0006` | Manager + all modules | ISSUE-0004 |
| ISSUE-0007 | gpt-5.5-medium; escalate to gpt-5.5-xhigh if Codec2/FreeDV internals block progress | `research/ISSUE-0007-codec2-submodule-pin` | `../f700f-wt-issue-0007` | Module 05 + Module 10 + Manager | ISSUE-0004 |
| ISSUE-0008 | gpt-5.5-medium; escalate to gpt-5.5-xhigh for interface boundary decisions | `feature/ISSUE-0008-mode-interface-v0-1` | `../f700f-wt-issue-0008` | Module 02 + Module 03 + Manager | ISSUE-0004 |

## Dispatch Order

1. ISSUE-0005: add GitHub Actions CI first so breakage is detected early.
2. ISSUE-0006: tighten docs/issues/reports governance for parallel operation.
3. ISSUE-0008: establish the Mode interface and minimal protocol boundary before downstream simulation work.
4. ISSUE-0007: pin Codec2/FreeDV so official 700D/700E baseline work has an authoritative source.

ISSUE-0005, ISSUE-0006, ISSUE-0007, and ISSUE-0008 may run in parallel from `develop` once each worker creates its issue branch and records TDD evidence before implementation.

## Hold Line

- Do not start ISSUE-0009 deterministic simulation core yet.
- Do not start SSB, 700D, 700E, or 700F implementation yet.
- ISSUE-0010 channel model may start only after ISSUE-0008 lands the minimal protocol needed by channel/simulation contracts.
- ISSUE-0009 and ISSUE-0011 through ISSUE-0015 should wait until ISSUE-0008 is integrated.

## Worker Instructions

- Use the branch and owner module recorded in the root issue file as the source of truth.
- Branch from local `develop`.
- Update the relevant root issue and module issue before implementation.
- Add or document the failing test/check before implementation.
- Run the relevant local checks before handoff.
- Record any push failure as a remote/permission blocker in the issue and handoff.

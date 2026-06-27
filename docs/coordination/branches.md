# Branch ownership

| Issue | Branch | Owner | Worktree | Status | Merge target |
|---|---|---|---|---|---|
| ISSUE-0003 | docs/ISSUE-0003-m1-planning | Manager | ../f700f-wt-issue-0003 | merged-local | develop |
| ISSUE-0004 | docs/ISSUE-0004-m0-audit | Manager / QA | ../f700f-wt-issue-0004 | final-checkpoint | develop |
| ISSUE-0005 | docs/ISSUE-0005-github-actions-ci | Module 01 / Manager | TBD | ready_m1a | develop |
| ISSUE-0006 | docs/ISSUE-0006-governance-structure | Manager + all modules | TBD | ready_m1a | develop |
| ISSUE-0007 | research/ISSUE-0007-codec2-submodule-pin | Manager + Module 05 + Module 10 | TBD | ready_m1a | develop |
| ISSUE-0008 | feature/ISSUE-0008-mode-interface-v0-1 | Module 02 + Module 03 + Manager | TBD | ready_m1a | develop |

## M1-A dispatch worktrees

| Issue | Worktree | Recommended model | Notes |
|---|---|---|---|
| ISSUE-0005 | ../f700f-wt-issue-0005 | GPT-5.3-Codex-Spark; gpt-5.5-low for CI failures; gpt-5.5-medium for Windows CI | Start from `develop`; CI should land early. |
| ISSUE-0006 | ../f700f-wt-issue-0006 | GPT-5.3-Codex-Spark | Start from `develop`; governance-only unless issue expands scope. |
| ISSUE-0007 | ../f700f-wt-issue-0007 | gpt-5.5-medium; gpt-5.5-xhigh for Codec2/FreeDV internals | Start from `develop`; no 700D/700E implementation yet. |
| ISSUE-0008 | ../f700f-wt-issue-0008 | gpt-5.5-medium; gpt-5.5-xhigh for interface boundary decisions | Start from `develop`; blocks ISSUE-0009 and most later work. |

Remote `develop` is not published. `origin` is configured, but previous push was blocked by GitHub permission.

# Project Issues

## Scope

- Root tracker for cross-module or cross-functional work.
- Module-local tracking remains in `modules/<NN>_<name>/issues/README.md`.

## Project issue policy

- Keep this index as the source of truth for project issue ownership and status.
- Issue-first rule: issue files must exist in the issue directory before implementation starts.
- Branch-first rule: branch/worktree entries in each issue file are source of truth for the issue owner.
- TDD-first rule: update issue `TDD record` before implementation for non-planning issues.

| ID | Status | Priority | Scope | Title | Owner | Branch | Target version | Milestone | Depends | Last updated |
|---|---|---|---|---|---|---|---|---|---|---|
| ISSUE-0001 | closed | P0 | project | Bootstrap repository | Manager | work | v0.0.1 | M0 | - | 2026-06-27 |
| ISSUE-0002 | closed | P0 | project | Fast push CI policy and workflows | Manager | work | v0.0.1 | M0 | ISSUE-0001 | 2026-06-27 |
| ISSUE-0003 | closed | P0 | project | M1 planning issue expansion | Manager | docs/ISSUE-0003-m1-planning | v0.1.0 | M1 | ISSUE-0001, ISSUE-0002 | 2026-06-27 |
| ISSUE-0004 | closed | P0 | qa | M0 audit and M1 go decision | Manager / QA | docs/ISSUE-0004-m0-audit | v0.1.0 | M1 | ISSUE-0001, ISSUE-0002 | 2026-06-27 |
| ISSUE-0005 | closed | P0 | project | GitHub Actions CI introduction | Module 01 / Manager | docs/ISSUE-0005-github-actions-ci | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0006 | closed | P0 | docs | Docs issues reports governance structure | Manager + all modules | docs/ISSUE-0006-governance-structure | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0007 | closed | P0 | dependency | external codec2 submodule pin | Manager + Module 05 + Module 10 | research/ISSUE-0007-codec2-submodule-pin | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0008 | closed | P0 | protocol | Common Mode interface and data protocol v0.1.0 | Module 02 + Module 03 + Manager | feature/ISSUE-0008-mode-interface-v0-1 | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0009 | closed | P0 | simulation | Deterministic simulation core | Module 11 + Module 02 + Module 12 | feature/ISSUE-0009-deterministic-simulation-core | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0010 | closed | P0 | module:08 | Channel model v0.1.0 | Module 08 | feature/ISSUE-0010-channel-model-v0-1 | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0011 | open | P0 | module:10 | SSB reference emulator | Module 10 + Module 04 | feature/ISSUE-0011-ssb-reference-emulator | v0.1.0 | M1 | ISSUE-0008, ISSUE-0010 | 2026-06-28 |
| ISSUE-0012 | open | P0 | module:05,10 | FreeDV 700D 700E official baseline adapter | Module 05 + Module 10 | feature/ISSUE-0012-freedv-official-baseline-adapter | v0.1.0 | M1 | ISSUE-0007, ISSUE-0008 | 2026-06-28 |
| ISSUE-0013 | closed | P1 | module:07,10 | 700D 700E parameterized emulator skeleton | Module 07 + Module 10 | feature/ISSUE-0013-700d-700e-parameterized-emulator | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0014 | closed | P0 | module:12 | Metrics and result artifact format | Module 12 + Module 15 | feature/ISSUE-0014-metrics-artifact-format | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0015 | closed | P0 | module:13 | Sweep runner v0.1.0 | Module 13 + Module 11 + Module 15 | feature/ISSUE-0015-sweep-runner-v0-1 | v0.1.0 | M1 | ISSUE-0009, ISSUE-0014 | 2026-06-28 |
| ISSUE-0016 | open | P0 | qa | M1 audit report | Manager + QA | docs/ISSUE-0016-m1-audit-report | v0.1.0 | M1 | ISSUE-0004 through ISSUE-0015 | 2026-06-27 |

## M1-A integration status

- ISSUE-0003 is integrated into local `develop` through `eba941a`.
- ISSUE-0004 M0 audit passed; reports exist under root `reports/` and Module 16 QA `reports/`.
- ISSUE-0005, ISSUE-0006, ISSUE-0007, and ISSUE-0008 are integrated locally into `develop`.
- ISSUE-0009, ISSUE-0010, ISSUE-0012, and ISSUE-0013 are startable from local `develop`.
- ISSUE-0011 remains held until ISSUE-0010 is complete.
- Remote `origin` is configured, but prior push was blocked by GitHub permission. Remote `develop` is not published.

## M1-B integration status

- ISSUE-0014, ISSUE-0010, ISSUE-0013, and ISSUE-0009 are integrated locally into `develop`.
- The M1-B instruction text used ISSUE-0013 for metrics and ISSUE-0012 for emulator, but existing issue files are source of truth: metrics is ISSUE-0014 and emulator is ISSUE-0013.
- ISSUE-0011, ISSUE-0012, and ISSUE-0015 are startable from local `develop`.
- ISSUE-0012 workers must initialize `external/codec2` before official adapter work.
- ISSUE-0016 remains held until remaining M1 work completes.

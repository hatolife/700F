# M1 Baseline Simulation Milestone

## Target version

- Project target: `v0.1.0`
- Completion status: `GO_WITH_LIMITATIONS` as of ISSUE-0016 on 2026-06-28
- Compatibility line: `1`
- Merge target: `develop`
- Master branch: not used for M1 day-to-day work.

## Purpose

M1 does not implement or optimize 700F itself. M1 prepares a deterministic baseline simulation harness that can compare:

- SSB standard 3k baseline
- SSB narrow 1.9k baseline
- FreeDV 700D official baseline
- FreeDV 700E official baseline
- 700D parameterized emulator
- 700E parameterized emulator
- Future 700F candidates through a common Mode interface

Parameterized 700D/700E emulators are explicitly not official baselines. Their gaps, assumptions, and validation state must be documented against the pinned Codec2/FreeDV source once `external/codec2` is available.

## Planned root issues

| Issue | Title | Primary owner | Branch | Target modules |
|---|---|---|---|---|
| ISSUE-0004 | M0 audit and M1 go decision | Manager / QA | `docs/ISSUE-0004-m0-audit` | project |
| ISSUE-0005 | GitHub Actions CI introduction | Module 01 / Manager | `docs/ISSUE-0005-github-actions-ci` | 01 |
| ISSUE-0006 | Docs issues reports governance structure | Manager + all modules | `docs/ISSUE-0006-governance-structure` | all docs |
| ISSUE-0007 | external codec2 submodule pin | Manager + Modules 05/10 | `research/ISSUE-0007-codec2-submodule-pin` | 05, 10 |
| ISSUE-0008 | Common Mode interface and data protocol v0.1.0 | Modules 02/03 + Manager | `feature/ISSUE-0008-mode-interface-v0-1` | 02, 03 |
| ISSUE-0009 | Deterministic simulation core | Modules 11/02/12 | `feature/ISSUE-0009-deterministic-simulation-core` | 11, 02, 12 |
| ISSUE-0010 | Channel model v0.1.0 | Module 08 | `feature/ISSUE-0010-channel-model-v0-1` | 08 |
| ISSUE-0011 | SSB reference emulator | Modules 10/04 | `feature/ISSUE-0011-ssb-reference-emulator` | 10, 04 |
| ISSUE-0012 | FreeDV 700D 700E official baseline adapter | Modules 05/10 | `feature/ISSUE-0012-freedv-official-baseline-adapter` | 05, 10 |
| ISSUE-0013 | 700D 700E parameterized emulator skeleton | Modules 07/10 | `feature/ISSUE-0013-freedv-parameterized-emulator` | 07, 10 |
| ISSUE-0014 | Metrics and result artifact format | Modules 12/15 | `feature/ISSUE-0014-metrics-artifact-format` | 12, 15 |
| ISSUE-0015 | Sweep runner v0.1.0 | Modules 13/11/15 | `feature/ISSUE-0015-sweep-runner-v0-1` | 13, 11, 15 |
| ISSUE-0016 | M1 audit report | Manager / QA | `docs/ISSUE-0016-m1-audit-report` | project |

## Branch and TDD rules

Each implementation issue must create its own branch from `develop` and use a matching worktree when run in parallel. The required order is issue creation, branch creation, docs/spec update, failing test, minimal implementation, passing tests, edge-case tests, docs update, local CI, handoff, commit, and merge request or direct branch push depending on current repository policy.

## M1-A entry gate

ISSUE-0004 M0 audit passed locally. M1-A may begin with ISSUE-0005, ISSUE-0006, ISSUE-0007, and ISSUE-0008 from local `develop`.

ISSUE-0009 and later simulation core/channel/SSB/700D/700E work should not begin until ISSUE-0008 lands the minimal Mode interface and data/plugin protocol baseline. ISSUE-0010 channel model may begin after that minimal protocol is available.

Remote `origin` is configured, but remote `develop` is not published because prior push was blocked by GitHub permission. Local workers should treat local `develop` as the dispatch source until remote publication is available.

## Version impact plan

M1 project version is `v0.1.0`. Interface-bearing or M1-touched modules moved to `v0.1.0` where applicable. Untouched modules may remain `v0.0.1` until they join a later compatibility line.

## Audit decision

ISSUE-0016 completed M1 with `GO_WITH_LIMITATIONS`.

Proceed to M2 planning and candidate exploration only with these limitations visible:

- SSB received-audio WAV/raw float export remains follow-up work.
- Official FreeDV 700D/700E waveform encode/decode and sample-audio round trip validation remain follow-up work.
- Sweep runner standalone CLI and TOML parsing remain follow-up work.
- Remote publication is not complete from this workspace because prior push was blocked by GitHub permission.

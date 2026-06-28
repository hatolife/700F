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
| ISSUE-0011 | closed | P0 | module:10 | SSB reference emulator | Module 10 + Module 04 | feature/ISSUE-0011-ssb-reference-emulator | v0.1.0 | M1 | ISSUE-0008, ISSUE-0010 | 2026-06-28 |
| ISSUE-0012 | closed | P0 | module:05,10 | FreeDV 700D 700E official baseline adapter | Module 05 + Module 10 | feature/ISSUE-0012-freedv-official-baseline-adapter | v0.1.0 | M1 | ISSUE-0007, ISSUE-0008 | 2026-06-28 |
| ISSUE-0013 | closed | P1 | module:07,10 | 700D 700E parameterized emulator skeleton | Module 07 + Module 10 | feature/ISSUE-0013-700d-700e-parameterized-emulator | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0014 | closed | P0 | module:12 | Metrics and result artifact format | Module 12 + Module 15 | feature/ISSUE-0014-metrics-artifact-format | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0015 | closed | P0 | module:13 | Sweep runner v0.1.0 | Module 13 + Module 11 + Module 15 | feature/ISSUE-0015-sweep-runner-v0-1 | v0.1.0 | M1 | ISSUE-0009, ISSUE-0014 | 2026-06-28 |
| ISSUE-0016 | closed | P0 | qa | M1 audit report | Manager + QA | docs/ISSUE-0016-m1-audit-report | v0.1.0 | M1 | ISSUE-0004 through ISSUE-0015 | 2026-06-28 |
| ISSUE-0017 | closed | P0 | project | M2 planning issue expansion | Manager | docs/ISSUE-0017-m2-planning | v0.2.0 target | M2 | ISSUE-0016 | 2026-06-28 |
| ISSUE-0018 | closed | P0 | architecture | 700F candidate architecture RFC | Module 02 + Manager | docs/ISSUE-0018-700f-candidate-architecture-rfc | v0.2.0 | M2 | ISSUE-0017 | 2026-06-28 |
| ISSUE-0019 | open | P0 | module:06 | FEC UEP engine prototype | Module 06 + Module 02 | feature/ISSUE-0019-fec-uep-prototype | v0.2.0 target | M2 | ISSUE-0018 | 2026-06-28 |
| ISSUE-0020 | open | P0 | module:07 | OFDM waveform candidate prototype | Module 07 + Module 09 | feature/ISSUE-0020-ofdm-waveform-candidate | v0.2.0 target | M2 | ISSUE-0018 | 2026-06-28 |
| ISSUE-0021 | open | P0 | module:09 | sync and impairment estimator prototype | Module 09 + Module 07 | feature/ISSUE-0021-sync-impairment-estimator | v0.2.0 target | M2 | ISSUE-0018, ISSUE-0020 | 2026-06-28 |
| ISSUE-0022 | open | P1 | module:14 | AI-assisted sweep triage | Module 14 + Module 13 | research/ISSUE-0022-ai-sweep-triage | v0.2.0 target | M2 | ISSUE-0015, ISSUE-0018 | 2026-06-28 |
| ISSUE-0023 | open | P0 | qa | M2 audit and candidate downselect | Manager + QA | docs/ISSUE-0023-m2-audit-downselect | v0.2.0 target | M2 | ISSUE-0018 through ISSUE-0022 | 2026-06-28 |
| ISSUE-0024 | open | P0 | module:12 | M2 scoring model and comparison policy | Module 12 + Module 13 | feature/ISSUE-0024-scoring-model | v0.2.0 target | M2 | ISSUE-0018 | 2026-06-28 |
| ISSUE-0025 | closed | P0 | module:08 | M2 expanded channel condition matrix | Module 08 + Module 13 | feature/ISSUE-0025-expanded-channel-matrix | v0.2.0 | M2 | ISSUE-0018, ISSUE-0010 | 2026-06-28 |
| ISSUE-0026 | open | P0 | module:13 | M2 sweep campaign runner config | Module 13 + Module 11 | feature/ISSUE-0026-m2-sweep-campaign-config | v0.2.0 target | M2 | ISSUE-0018, ISSUE-0025 | 2026-06-28 |
| ISSUE-0027 | open | P0 | module:15 | M2 baseline comparison report generation | Module 15 + Module 12 | feature/ISSUE-0027-baseline-comparison-report | v0.2.0 target | M2 | ISSUE-0018, ISSUE-0024, ISSUE-0026 | 2026-06-28 |

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

## M1-C integration status

- ISSUE-0011, ISSUE-0012, and ISSUE-0015 are integrated locally into `develop`.
- Merge commits: ISSUE-0011 `0f09a49`, ISSUE-0012 `5b48e62`, ISSUE-0015 `1710785`.
- Final M1-C local CI passed with 9 CTest tests.
- Push/PR not performed from this workspace; prior publication is blocked by GitHub permission.
- ISSUE-0016 M1 audit is now startable from local `develop`.

## M1 audit decision

- ISSUE-0016 completed with decision `GO_WITH_LIMITATIONS`.
- Root project version and M1-touched module versions are recorded as `v0.1.0` where applicable.
- M2 planning may begin, but official FreeDV waveform execution, SSB audio export, sweep CLI/TOML parsing, and remote publication remain follow-up items.

## M2 planning status

- ISSUE-0017 through ISSUE-0023 define M2 700F candidate exploration.
- M2-A startable planning/design work: ISSUE-0018 after ISSUE-0017 is merged.
- M2 implementation issues ISSUE-0019, ISSUE-0020, ISSUE-0021, and ISSUE-0022 must not begin candidate code until ISSUE-0018 defines boundaries and test expectations.
- ISSUE-0023 remains held until M2 candidate work completes.

## M2-A integration status

- ISSUE-0018 is integrated locally into `develop` as merge commit `59629d4`.
- Local CI after ISSUE-0018 passed with 10 CTest tests.
- A later manager request asked to use ISSUE-0019 through ISSUE-0022 for scoring/channel/sweep/report, but those issue numbers already exist as FEC/OFDM/sync/AI triage. Existing issue files remain source of truth.
- Requested scoring/channel/sweep/report work is split into ISSUE-0024 through ISSUE-0027.
- ISSUE-0023 remains held until the current M2 implementation issue set and follow-up comparison/report issue set are resolved or explicitly reprioritized.

## M2 issue map

- `docs/coordination/m2_issue_map.md` records the canonical M2 issue-number mapping.
- Future requests that conflict with existing issue meaning should create new follow-up issues rather than redefining existing issue files.

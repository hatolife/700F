# M2 700F Candidate Exploration Milestone

## Target version

- Project target: `v0.2.0`
- Compatibility line: `2`
- Merge target: `develop`
- Entry condition: ISSUE-0016 M1 audit decision `GO_WITH_LIMITATIONS`
- Master branch: not used for M2 day-to-day work.

## Purpose

M2 begins 700F candidate exploration without claiming final modem performance. The goal is to create bounded, testable prototypes for the core candidate dimensions:

- 700F candidate architecture and interface boundaries
- FEC/UEP engine prototype
- OFDM waveform candidate prototype
- sync/impairment estimator prototype
- sweep triage support for comparing candidates against M1 baselines

M2 must preserve clear separation between official baselines, parameterized emulators, and new 700F candidate modes.

## Planned root issues

| Issue | Title | Primary owner | Branch | Target modules |
|---|---|---|---|---|
| ISSUE-0017 | M2 planning issue expansion | Manager | `docs/ISSUE-0017-m2-planning` | project |
| ISSUE-0018 | 700F candidate architecture RFC | Module 02 + Manager | `docs/ISSUE-0018-700f-candidate-architecture-rfc` | 02, 05, 06, 07, 09, 10, 11, 12, 13 |
| ISSUE-0019 | FEC UEP engine prototype | Module 06 + Module 02 | `feature/ISSUE-0019-fec-uep-prototype` | 06, 02 |
| ISSUE-0020 | OFDM waveform candidate prototype | Module 07 + Module 09 | `feature/ISSUE-0020-ofdm-waveform-candidate` | 07, 09 |
| ISSUE-0021 | sync and impairment estimator prototype | Module 09 + Module 07 | `feature/ISSUE-0021-sync-impairment-estimator` | 09, 07 |
| ISSUE-0022 | AI-assisted sweep triage | Module 14 + Module 13 | `research/ISSUE-0022-ai-sweep-triage` | 14, 13 |
| ISSUE-0023 | M2 audit and candidate downselect | Manager + QA | `docs/ISSUE-0023-m2-audit-downselect` | 16, project |
| ISSUE-0024 | M2 scoring model and comparison policy | Module 12 + Module 13 | `feature/ISSUE-0024-scoring-model` | 12, 13 |
| ISSUE-0025 | M2 expanded channel condition matrix | Module 08 + Module 13 | `feature/ISSUE-0025-expanded-channel-matrix` | 08, 13 |
| ISSUE-0026 | M2 sweep campaign runner config | Module 13 + Module 11 | `feature/ISSUE-0026-m2-sweep-campaign-config` | 13, 11 |
| ISSUE-0027 | M2 baseline comparison report generation | Module 15 + Module 12 | `feature/ISSUE-0027-baseline-comparison-report` | 15, 12 |
| ISSUE-0028 | M2-C campaign mode registry bridge | Module 13 + Module 10 + Module 02 | `feature/ISSUE-0028-m2-campaign-mode-registry` | 13, 10, 02 |

## M2-A Gate

ISSUE-0018 is the first M2-A issue. It must define the candidate-mode boundary, minimal data contracts, test expectations, and baseline comparison rules before M2 implementation branches start.

ISSUE-0019 through ISSUE-0022 may prepare research notes and red-test plans, but should not implement candidate behavior until ISSUE-0018 is integrated.

ISSUE-0024 through ISSUE-0027 cover scoring, channel-matrix, sweep-campaign, and baseline-comparison report work requested after ISSUE-0018. They exist because ISSUE-0019 through ISSUE-0022 were already assigned to FEC/OFDM/sync/AI triage in the repository issue tracker.

The canonical issue-number mapping is recorded in `docs/coordination/m2_issue_map.md`. If a later instruction collides with an existing issue number, keep the existing issue and create a new follow-up issue for the new work.

M2-B integrated ISSUE-0024 through ISSUE-0027 and produced smoke JSON/CSV artifacts. The smoke campaign currently records all rows as skipped because campaign modes are not registered in the smoke runner. ISSUE-0028 is the bridge from profile/config descriptors to registered mode factories.

## M1 Limitations Carried Into M2

- SSB received-audio WAV/raw float export is incomplete.
- Official FreeDV 700D/700E waveform encode/decode is incomplete.
- Sweep CLI and TOML parsing are incomplete.
- `external/codec2` is pinned but may be uninitialized in a fresh workspace.
- Remote publication from this workspace is blocked by GitHub permission.

## M2 Exit Criteria

- At least one 700F candidate path can run through the M1 pipeline with deterministic artifacts.
- Candidate output is clearly labeled as experimental and not an official baseline.
- FEC/UEP, waveform, and sync estimator assumptions are documented.
- Sweep comparison artifacts include M1 baselines or explicit skips with reasons.
- M2 audit records go/no-go or downselect recommendation for M3.

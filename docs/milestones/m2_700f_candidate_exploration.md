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

## M2-A Gate

ISSUE-0018 is the first M2-A issue. It must define the candidate-mode boundary, minimal data contracts, test expectations, and baseline comparison rules before M2 implementation branches start.

ISSUE-0019 through ISSUE-0022 may prepare research notes and red-test plans, but should not implement candidate behavior until ISSUE-0018 is integrated.

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

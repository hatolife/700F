# ISSUE-0014: Metrics and result artifact format

## Status
open

## Scope
module:12

## Priority
P0

## Owner
- Codex role: Module 12 + Module 15
- Recommended model: GPT-5.3-Codex-Spark

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0014-metrics-artifact-format
- Worktree: ../f700f-wt-issue-0014
- Merge target: develop
- PR/Merge status: none

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 12 and 15 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Define comparable result metrics output for SSB, 700D, 700E, and future 700F candidates.

## Affected paths
- modules/12_metrics_scoring/
- modules/15_reporting_visualization/
- docs/specs/metrics.md

## Dependencies
- ISSUE-0008

## Acceptance criteria
- [ ] mode_id, channel_id, seed, snr_db, freq_offset_hz, and result metrics are recorded.
- [ ] BER/FER are recorded where applicable.
- [ ] Audio-only modes represent BER/FER as N/A.
- [ ] dropout rate, sync loss count, latency estimate, and audio export path can be recorded.
- [ ] Schema can later add ASR WER, STOI, and ESTOI.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: to be completed by the issue owner before implementation.
- Red command: to be completed by the issue owner.
- Expected failure: to be completed by the issue owner.
- Green command: see Test plan.
- Refactor notes: to be completed by the issue owner.
- TDD exception if any: planning-only issue until implementation begins.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.

## Result
Pending.

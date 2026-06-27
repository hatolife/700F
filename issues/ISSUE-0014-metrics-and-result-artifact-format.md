# ISSUE-0014: Metrics and result artifact format

## Status
closed

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
- PR/Merge status: merged local to develop as `b518264`; push/PR not performed because GitHub permission is blocked

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
- [x] mode_id, channel_id, seed, snr_db, freq_offset_hz, and result metrics are recorded.
- [x] BER/FER are recorded where applicable.
- [x] Audio-only modes represent BER/FER as N/A.
- [x] dropout rate, sync loss count, latency estimate, and audio export path can be recorded.
- [x] Schema can later add ASR WER, STOI, and ESTOI.

## Test plan
- ctest --test-dir build --output-on-failure

## TDD record
- Red test added: `tests/test_metrics_result_artifacts.cpp` added with assertions for:
  - empty result object generation
  - SSB-like audio-only BER/FER being `N/A`
  - digital result recording BER/FER
  - skipped-result representation
  - stable JSON serialization order
  - CSV minimum columns
  - unknown optional metrics survivability
- Red command: `./tools/run_ci_local.sh` (expected to fail before implementation).
- Expected failure: JSON/CSV formatting and result schema types were missing in `modules/12_metrics_scoring`.
- Green command: `./tools/run_ci_local.sh`.
- Refactor notes: keep JSON/CSV shape as append-only extension points for future ASR and intelligibility metrics.
- TDD exception: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-27 23:41 UTC Implementation start in worktree `C:/Users/user/Documents/f700f-wt-issue-0014` on branch `feature/ISSUE-0014-metrics-artifact-format`; TDD test plan recorded above.
- 2026-06-27 23:54 UTC `tests/test_metrics_result_artifacts.cpp` assertions added and executed; JSON parse hardening completed; `./tools/run_ci_local.sh` passes.
- 2026-06-27 14:59 UTC Manager merged branch into `develop` as `b518264`; post-merge local CI passed.

## Result
Implemented a minimal but stable JSON/CSV result artifact format for module 12 and documented it in `docs/specs/metrics.md`. Added helpers for tolerant parsing, optional metric columns, N/A handling for BER/FER, skip/error summary fields, and compatibility-forward optional metrics payloads.

Artifacts now include required project/mode/channel metadata, run parameters, BER/FER behavior, dropouts/sync, timing, and optional metrics without breaking unknown keys. Branch is merged locally to `develop`.

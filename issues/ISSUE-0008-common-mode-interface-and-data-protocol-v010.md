# ISSUE-0008: Common Mode interface and data protocol v0.1.0

## Status
closed

## Scope
protocol

## Priority
P0

## Owner
- Codex role: Module 02 + Module 03 + Manager
- Recommended model: gpt-5.5-medium; escalate to gpt-5.5-xhigh for boundary decisions

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0008-mode-interface-v0-1
- Worktree: ../f700f-wt-issue-0008
- Merge target: develop
- PR/Merge status: merged local to develop as `67c6327`; push/PR not performed because GitHub permission is blocked

## Target version and milestone
- Project version: v0.1.0 target for M1 unless noted
- Module version: project target v0.1.0; modules 02 and 03 target v0.1.0
- Compatibility line: 1 for M1-touched interfaces/modules
- Milestone: M1 Baseline Simulation

## Submodule impact
none

## Background
M1 builds the baseline simulation environment needed before 700F candidate exploration.

## Task
Define the shared Mode interface and data protocol for SSB, 700D, 700E, and future 700F candidates.

## Affected paths
- include/f700f/
- docs/specs/mode_interface.md
- docs/specs/data_protocol.md
- docs/specs/plugin_protocol.md
- modules/02_core_data_model_protocol/
- modules/03_plugin_abi_registry/

## Dependencies
- ISSUE-0004

## Acceptance criteria
- [x] Minimal encode/decode/describe/reset interface is defined.
- [x] AudioBlock, ComplexBlock, BitBlock, FrameStatus, and ModeDescriptor responsibilities are documented.
- [x] Mode descriptor exposes bandwidth, latency, sample rate, codec name, and FEC name.
- [x] C++ interface and C ABI plugin relationship is documented.
- [x] Existing smoke test passes.
- [x] Affected module versions update to v0.1.0.

## Test plan
- ctest --test-dir build --output-on-failure
- python3 tools/check_versions.py

## TDD record
- Red test added: planned `tests/test_mode_interface.cpp` coverage for dummy descriptor retrieval, encode/decode, reset, invalid config, zero-length block, SSB-like descriptor representation, and FreeDV-like descriptor representation.
- Red command: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ctest --test-dir build --output-on-failure`
- Expected failure: confirmed under bash; `tests/test_mode_interface.cpp` fails to compile because `ModeId`, `ModeCapabilities`, `ModeRuntimeConfig`, `EncodeResult`, `DecodeResult`, `IMode`, `IModeFactory`, and `ModeRegistry` are absent from the public C++20 header.
- Green command: `bash ./tools/run_ci_local.sh`
- Refactor notes: Kept implementation header-only and minimal; no concrete mode, Codec2, SSB, 700D/700E, channel model, plugin loader, or ISSUE-0009+ behavior was added.
- TDD exception if any: none.

## Work log
- 2026-06-27 00:00 UTC Created by Manager Codex during M1 planning expansion.
- 2026-06-27 23:40 JST Subagent C started local worktree implementation on `feature/ISSUE-0008-mode-interface-v0-1`; push remains blocked by task instruction, so handoff will be local commit only.
- 2026-06-28 00:05 JST Added failing Mode interface contract tests, implemented minimal public C++20 interface, documented mode/data/plugin protocols, updated module 02/03 versions, and passed local CI.
- 2026-06-28 00:05 JST Git status before commit: `## feature/ISSUE-0008-mode-interface-v0-1` with modified tracked files and new `docs/specs/`, `tests/`.
- 2026-06-27 14:33 UTC Manager merged branch `feature/ISSUE-0008-mode-interface-v0-1` into `develop` as `67c6327`; post-merge `bash ./tools/run_ci_local.sh` passed.

## Result
Local implementation complete and merged locally to `develop`. Push/PR intentionally not performed per task instruction.

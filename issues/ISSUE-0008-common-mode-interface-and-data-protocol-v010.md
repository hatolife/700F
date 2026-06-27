# ISSUE-0008: Common Mode interface and data protocol v0.1.0

## Status
open

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
- PR/Merge status: none

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
- [ ] Minimal encode/decode/describe/reset interface is defined.
- [ ] AudioBlock, ComplexBlock, BitBlock, FrameStatus, and ModeDescriptor responsibilities are documented.
- [ ] Mode descriptor exposes bandwidth, latency, sample rate, codec name, and FEC name.
- [ ] C++ interface and C ABI plugin relationship is documented.
- [ ] Existing smoke test passes.
- [ ] Affected module versions update to v0.1.0.

## Test plan
- ctest --test-dir build --output-on-failure
- python3 tools/check_versions.py

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

# ISSUE-0033: 700D/700E emulator minimal behavior bridge

## Status
complete

## Scope
module:10,11,13

## Priority
P0

## Owner
- Codex role: Module 10 + Module 11 + Module 13
- Recommended model: gpt-5.5-xhigh

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0033-700d-700e-emulator-minimal-behavior
- Worktree: ../700F-ISSUE-0033
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0029
- ISSUE-0030
- ISSUE-0031

## Task
Move 700D/700E emulator rows beyond descriptor-only into deterministic minimal
behavior while clearly labeling them as non-official and non-downselect-valid.

## Acceptance criteria
- [x] Behavior is deterministic.
- [x] Results clearly state non-official emulator status.
- [x] Reports do not present emulator rows as official FreeDV performance.
- [x] Local CI passes.

## TDD record
- 2026-06-29 JST Planned red tests before implementation:
  `freedv_emulator_contract` should require deterministic successful
  encode/decode for `freedv700d_emulated` and `freedv700e_emulated` with
  `implementation_status = emulated_surrogate`; `sweep_runner_contract` and
  `report_artifact_loader_cli_contract` should require completed
  `emulated_surrogate` rows that carry `official=false`,
  `not_official_freedv=true`, `performance_valid=false`,
  `downselect_valid=false`, emulator model metadata, and limitation text;
  unknown channel/mode skips should remain clear.
- 2026-06-29 JST Focused contracts passed after implementation:
  `freedv_emulator_contract`, `sweep_runner_contract`,
  `report_artifact_loader_cli_contract`, `scoring_model_contract`, and
  `baseline_comparison_report_contract`.

## Result
- Implemented deterministic `emulated_surrogate` encode/decode behavior for
  `freedv700d_emulated` and `freedv700e_emulated`.
- Sweep/report/scoring paths now label emulator rows with non-official,
  performance-invalid, and downselect-invalid metadata while keeping official
  FreeDV rows skipped under default Codec2-disabled builds.
- Local CI: `bash ./tools/run_ci_local.sh` passed on 2026-06-29 JST
  (20/20 CTest tests plus version and governance checks). Non-blocking WSL
  `/mnt/c` clock-skew warnings were observed during build.

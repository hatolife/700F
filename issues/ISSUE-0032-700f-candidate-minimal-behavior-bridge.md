# ISSUE-0032: 700F candidate minimal behavior bridge

## Status
complete

## Scope
module:02,06,07,09,11,13

## Priority
P0

## Owner
- Codex role: Module 02 + candidate implementation modules
- Recommended model: gpt-5.5-xhigh

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0032-700f-candidate-minimal-behavior
- Worktree: ../700F-ISSUE-0032
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0029
- ISSUE-0030
- ISSUE-0031

## Task
Connect 700F profile-only candidates to clearly labeled minimal surrogate
behavior without claiming real modem performance.

## Acceptance criteria
- [x] Results include `surrogate`, `not_real_modem`, and `not_downselect_valid`.
- [x] Reports do not treat surrogate rows as RF performance.
- [x] Any interface expansion is documented before implementation.
- [x] Local CI passes.

## TDD record
- 2026-06-29: Start with failing coverage for 700F A/B/C surrogate rows completing
  without real modem claims. Tests must assert `implementation_status = surrogate`,
  `not_real_modem = true`, `downselect_valid = false`, `performance_valid = false`,
  surrogate model metadata, synthetic metric labeling, BER/FER non-performance
  handling, report warnings, and scoring separation between real performance and
  surrogate/readiness evidence.
- 2026-06-29: Red build failed on the new snapshot and sweep-row fields before
  implementation. Green verification passed with `bash ./tools/run_ci_local.sh`
  after implementation.

## Result
Implemented. The 700F A/B/C candidates now register as `implementation_status =
surrogate` and sweep as completed `surrogate_completed` rows with
`not_real_modem = true`, `downselect_valid = false`, `not_downselect_valid = true`,
`performance_valid = false`, surrogate model metadata, and synthetic readiness
labeling. Reports emit a surrogate warning and keep real performance score separate
from synthetic surrogate readiness score. BER/FER are not emitted as real surrogate
performance values. Real candidate downselect remains prohibited.

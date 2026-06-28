# ISSUE-0041: real 700F modem architecture RFC

## Status
complete

## Scope
module:02

## Priority
P0

## Owner
- Codex role: Module 02 + Manager

## Branch
- Base branch: develop
- Work branch: docs/ISSUE-0041-real-700f-modem-architecture-rfc
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: v0.2.0 target
- Milestone: M2 real modem RFC

## Task
Define the protocol/data boundary for the first real 700F modem prototype.

## Acceptance criteria
- [x] `ModemInputFrame`, `ModemOutputFrame`, `SymbolBlock`,
  `BasebandFrame`, and `PrototypeFrameStatus` are defined at RFC level.
- [x] Prototype metadata and status semantics are defined.
- [x] Real downselect remains blocked.
- [x] Follow-up implementation issues are separated from this design issue.

## Test plan
- `bash ./tools/run_ci_local.sh`
- `bash ./tools/run_codec2_on_ci_local.sh`

## TDD record
- TDD exception: RFC/design-only issue.

## Result
Complete. Module 02 owns the protocol boundary described in
`docs/specs/700f_modem_boundaries.md`; implementation starts only in follow-up
issues.

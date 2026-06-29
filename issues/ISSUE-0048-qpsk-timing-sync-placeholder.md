# ISSUE-0048: QPSK timing/sync placeholder

- Status: open
- Priority: P1
- Scope: module:09,07,02
- Owner: Module 09 + Module 07 + Module 02
- Branch: feature/ISSUE-0048-qpsk-timing-sync-placeholder
- Target version: v0.3.0 target
- Milestone: M3-B sync placeholder
- Depends: ISSUE-0045, ISSUE-0046
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

Timing and carrier synchronization must be represented before later real modem
work can evaluate impairments, but M3-A should not implement final sync.

## Acceptance Criteria

- [ ] Define placeholder timing/carrier sync status metadata.
- [ ] Document how sync placeholder status maps to prototype diagnostics.
- [ ] Keep real synchronization implementation out of scope.
- [ ] Keep real downselect prohibited.

## TDD Record

- Planned for M3-B; no implementation in ISSUE-0045.

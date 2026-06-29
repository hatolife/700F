# ISSUE-0049: FEC placeholder and UEP policy RFC

- Status: closed
- Priority: P0
- Scope: module:06
- Owner: Module 06 + Module 02 + Module 12 + Module 15
- Branch: docs/ISSUE-0049-fec-placeholder-uep-policy-rfc
- Target version: v0.3.0 target
- Last updated: 2026-06-29

## Module Scope

Module 06 owns FEC placeholder and UEP policy vocabulary. M3-A must not
implement real FEC.

## Acceptance Criteria

- [x] Root ISSUE-0049 acceptance criteria relevant to Module 06 are satisfied.
- [x] No-FEC, placeholder-FEC, and future real-FEC states are distinct.
- [x] Real downselect remains prohibited.

## TDD Record

- If source descriptors are added, start with tests listed in root ISSUE-0049.
- 2026-06-29: Started as RFC/documentation work. Planning-first exception
  applies while no source descriptors are added.
- 2026-06-29: Completed as documentation-only RFC work. No source descriptors
  were added, so descriptor tests were not required.

## Completion Notes

- Module 06 now has documented no-FEC, placeholder-FEC, future real-FEC, UEP
  class, latency, interleaver-reference, and report-validity vocabulary.
- Real FEC implementation, interleaver behavior, and coding-family downselect
  remain future scoped work.

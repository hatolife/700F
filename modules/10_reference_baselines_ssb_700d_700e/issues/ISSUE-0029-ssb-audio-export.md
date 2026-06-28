# ISSUE-0029: SSB audio export

## Status
open

## Scope
module:10

## Branch
- Work branch: feature/ISSUE-0029-ssb-audio-export

## Task
Expose SSB reference output audio artifact support for the M2-D smoke/report path.

## Acceptance criteria
- [x] SSB standard/narrow rows can record audio artifact paths when export is enabled.
- [x] Disabled export is reported as N/A.
- [x] Zero-length audio does not crash.

## TDD record
- 2026-06-29: Marked done by simulation/sweep aggregate propagation in module 11/13.

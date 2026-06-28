# ISSUE-0031: report JSON loader and CLI

## Status
open

## Scope
module:15

## Branch
- Work branch: feature/ISSUE-0031-report-json-loader-cli

## Task
Load sweep artifact JSON/CSV and generate comparison Markdown through a CLI.

## Acceptance criteria
- [x] `--help` succeeds.
- [x] Sample JSON generates Markdown.
- [x] Skipped/profile-only/descriptor-only rows are shown.
- [x] Report states whether real downselect is possible.

## TDD record
- 2026-06-29 JST: Started on
  `feature/ISSUE-0031-report-json-loader-cli`. Red tests will cover aggregate
  JSON/CSV loading, CLI help/error behavior, Markdown generation, and explicit
  real-downselect feasibility wording.
- 2026-06-29 JST: Added loader/CLI tests before implementation. Report-specific
  CTest slice passes; full local CI is blocked by unrelated ISSUE-0030
  sweep-loader tests.

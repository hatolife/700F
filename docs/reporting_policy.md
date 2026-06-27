# Reporting policy

## Filename format

All issue/decision reports use UTC-based filenames:

`YYYYMMDD_HHMMSS_<topic>.md`

Examples:

- `20260627_141022_issue_0006_governance_policy.md`
- `20260627_154501_ci_gate_check_results.md`

## Report scope

- **Root reports (`reports/`)**: project-level checkpoints, audits, and cross-module decisions.
- **Module reports (`modules/<NN>_<name>/reports/`)**: module-owned validation notes, implementation decisions, and local QA/audit outputs.

## Timing

Create reports when the work reaches one of these states:

- decision point reached (architecture choice, policy decision, compatibility change),
- pre-merge handoff,
- test/audit outcomes are available and should be persisted.

## Required index update

After creating or editing a report file:

- add or update the corresponding row in the right reports index (`reports/README.md` or module reports README),
- include related issue IDs and a short decision summary.

## Governance note

Reports for module issues should be referenced from the module issue when needed, but remain in the owning module directory.

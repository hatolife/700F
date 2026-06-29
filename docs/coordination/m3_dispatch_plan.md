# M3 Dispatch Plan

## Source Of Truth

Existing root issue files are the source of truth for branch names, owners,
dependencies, and acceptance criteria.

## Dispatch Table

| Issue | Recommended model | Branch | Worktree | Owner module | Predecessors |
|---|---|---|---|---|---|
| ISSUE-0046 | gpt-5.5-xhigh | `feature/ISSUE-0046-ofdm-carrier-layout-baseband-frame` | `../700F-ISSUE-0046` | Module 07 | ISSUE-0045, ISSUE-0043, ISSUE-0042 |
| ISSUE-0047 | gpt-5.5-medium; fallback gpt-5.5-xhigh | `feature/ISSUE-0047-occupied-bandwidth-validation` | `../700F-ISSUE-0047` | Module 12 + Module 15 | ISSUE-0045, ISSUE-0046 |
| ISSUE-0049 | gpt-5.5-medium; fallback gpt-5.5-xhigh | `docs/ISSUE-0049-fec-placeholder-uep-policy-rfc` | `../700F-ISSUE-0049` | Module 06 | ISSUE-0045, ISSUE-0044 |
| ISSUE-0051 | gpt-5.5-medium; fallback gpt-5.5-xhigh | `feature/ISSUE-0051-channel-impairment-regression` | `../700F-ISSUE-0051` | Module 08 + Module 13 | ISSUE-0045, ISSUE-0047 |

## Merge Order

1. ISSUE-0049.
2. ISSUE-0046.
3. ISSUE-0047.
4. ISSUE-0051.

## Hold

ISSUE-0048, ISSUE-0050, ISSUE-0052, and ISSUE-0053 are not M3-A first-wave
tasks. They should wait until the M3-A boundary and measurement work is merged.

## Guardrails

Every M3-A branch must keep default Codec2 OFF CI and optional Codec2 ON CI
green. Every branch must preserve `downselect_valid=false` for 700F prototype
rows.

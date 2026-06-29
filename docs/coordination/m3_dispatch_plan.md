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

ISSUE-0048, ISSUE-0050, ISSUE-0052, and ISSUE-0053 were held during the M3-A
first wave. They are now the next startable M3-B/audit set after the M3-A
boundary and measurement work merged.

## Guardrails

Every M3-A branch must keep default Codec2 OFF CI and optional Codec2 ON CI
green. Every branch must preserve `downselect_valid=false` for 700F prototype
rows.

ISSUE-0049 is RFC-only unless it adds a tiny descriptor/test patch. It must
define `fec_family=none` versus `fec_family=placeholder`, future real-FEC
exclusion, UEP policy metadata, important/less-important bit classes,
latency/interleaver references, and report representation before ISSUE-0046 or
ISSUE-0050 rely on the vocabulary.

## Integration Result

M3-A integrated into `develop` in the planned order:

1. ISSUE-0049: merge `2c8cef4`.
2. ISSUE-0046: merge `3ca00da`.
3. ISSUE-0047: merge `f5f3180`.
4. ISSUE-0051: merge `7e095dd`.

Default Codec2 OFF and optional Codec2 ON local CI passed after each merge.
M3-A smoke passed in both paths. Real downselect remains prohibited.

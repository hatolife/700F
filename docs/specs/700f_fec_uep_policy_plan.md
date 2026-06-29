# 700F FEC/UEP Policy Plan

## Status

- Issue: ISSUE-0045
- Follow-up: ISSUE-0049

## Purpose

Define the vocabulary and replacement boundary for future 700F FEC and unequal
error protection work.

## States

M3 planning distinguishes:

- `fec_family=none`: no FEC is present.
- `fec_family=placeholder`: metadata exists, but no real correction is applied.
- real FEC family names: future scoped issues only.

## UEP Policy Direction

Future UEP work should be able to classify payload regions such as:

- important bits
- less important bits
- frame/header bits
- payload/audio bits
- parity or future FEC metadata

ISSUE-0049 should document policy metadata without implementing the final code.

## Latency And Interleaver Relationship

FEC and interleaving must record latency assumptions. ISSUE-0050 owns the
interleaver placeholder; ISSUE-0049 should define how FEC metadata references
that future boundary.

## Reporting

Reports may show placeholder FEC status and UEP policy metadata. They must not
treat placeholder FEC as performance-valid evidence.

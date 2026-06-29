# ISSUE-0049: FEC placeholder and UEP policy RFC

- Status: open
- Priority: P0
- Scope: module:06,02,12,15
- Owner: Module 06 + Module 02 + Module 12 + Module 15
- Branch: docs/ISSUE-0049-fec-placeholder-uep-policy-rfc
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0044
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

M3 needs a replaceable FEC placeholder and UEP policy boundary before real FEC
or interleaver work starts.

## Acceptance Criteria

- [ ] Define no-FEC, placeholder-FEC, and real-FEC states.
- [ ] Define UEP policy metadata and future important-bit classes.
- [ ] Document latency and interleaver relationships.
- [ ] Describe metrics/report representation.
- [ ] Keep FEC implementation and real downselect out of scope.
- [ ] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planning/RFC-first. If source descriptors are added, start with tests for
  placeholder descriptor creation, `fec_family=placeholder`, no-FEC vs
  placeholder distinction, and UEP metadata retention.

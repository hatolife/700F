# ISSUE-0046: OFDM carrier layout and baseband frame prototype

- Status: open
- Priority: P0
- Scope: module:07,02,11,13
- Owner: Module 07 + Module 02 + Module 11 + Module 13
- Branch: feature/ISSUE-0046-ofdm-carrier-layout-baseband-frame
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0043, ISSUE-0042
- Recommended model: gpt-5.5-xhigh
- Last updated: 2026-06-29

## Problem

M3 needs an OFDM carrier layout and baseband frame boundary before later modem
work can add real OFDM modulation, timing sync, FEC, or interleaving.

## Acceptance Criteria

- [ ] Add a carrier layout descriptor for OFDM prototype planning.
- [ ] Preserve 1.9 kHz occupied bandwidth target metadata.
- [ ] Distinguish data, pilot, and guard carrier placeholders.
- [ ] Reject invalid carrier spacing, invalid occupied bandwidth, and zero
  carrier layouts.
- [ ] Preserve future FEC and sync placeholder metadata.
- [ ] Do not replace the ISSUE-0042 minimal QPSK runtime path with final OFDM.
- [ ] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planned red tests: carrier layout creation, invalid spacing rejection, invalid
  bandwidth rejection, zero carrier rejection, and placeholder metadata round
  trip.

## Notes

This issue is a boundary/prototype issue only. Real 700F downselect remains
prohibited.

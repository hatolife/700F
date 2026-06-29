# ISSUE-0051: channel impairment regression expansion

- Status: open
- Priority: P0
- Scope: module:08,13,11,15
- Owner: Module 08 + Module 13 + Module 11 + Module 15
- Branch: feature/ISSUE-0051-channel-impairment-regression
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0047
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

M3 needs a short, deterministic channel impairment regression subset before
OFDM/FEC/sync work expands runtime behavior.

## Acceptance Criteria

- [ ] Add or document an M3 smoke channel matrix.
- [ ] Include AWGN, frequency offset, and simple fading conditions.
- [ ] Keep seed handling deterministic.
- [ ] Preserve explicit unavailable/skipped reasons for Codec2 OFF official rows.
- [ ] Keep CI smoke short enough for routine regression.
- [ ] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planned red tests: matrix contains AWGN/frequency-offset/fading conditions,
  deterministic seeds, short smoke subset, and skip reason propagation.

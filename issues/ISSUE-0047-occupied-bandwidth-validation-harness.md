# ISSUE-0047: occupied bandwidth validation harness

- Status: open
- Priority: P0
- Scope: module:12,15,07,13
- Owner: Module 12 + Module 15 + Module 07 + Module 13
- Branch: feature/ISSUE-0047-occupied-bandwidth-validation
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0046
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

700F-A and future OFDM frames need a repeatable simulation-side occupied
bandwidth estimate before the project can compare bandwidth behavior in reports.

## Acceptance Criteria

- [ ] Add a deterministic bandwidth measurement utility or harness.
- [ ] Handle empty and zero signals safely.
- [ ] Reject invalid sample rates.
- [ ] Provide stable estimates for single-tone and two-tone test inputs.
- [ ] Record occupied bandwidth estimate fields in result/report paths.
- [ ] Document measurement limitations and 1.9 kHz target comparison.
- [ ] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planned red tests: zero signal handling, single-tone estimate stability,
  two-tone wider-than-single-tone estimate, invalid sample-rate rejection, empty
  input safety, artifact/report field propagation.

## Notes

This is not a regulatory-grade occupied bandwidth measurement. It is an M3-A
simulation harness for future comparison and regression.

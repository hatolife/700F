# ISSUE-0051: channel impairment regression expansion

- Status: done
- Priority: P0
- Scope: module:08
- Owner: Module 08 + Module 13 + Module 11 + Module 15
- Branch: feature/ISSUE-0051-channel-impairment-regression
- Target version: v0.3.0 target
- Last updated: 2026-06-29

## Module Scope

Module 08 owns the channel-condition side of the M3 regression expansion.

## Acceptance Criteria

- [x] Root ISSUE-0051 acceptance criteria relevant to Module 08 are satisfied.
- [x] AWGN, frequency-offset, and simple-fading conditions are represented.
- [x] CI smoke remains short and deterministic.

## TDD Record

- Planned red tests are recorded in root ISSUE-0051.
- 2026-06-29: Started TDD pass for the channel-condition matrix handoff.
  Expected first red checks require AWGN, frequency-offset, and simple-fading
  conditions in a short deterministic M3 smoke matrix.
- 2026-06-29: Completed handoff with
  `configs/channels/m3_channel_impairment_smoke.toml` and
  `configs/sweeps/m3_channel_impairment_smoke.toml`. Local Codec2 OFF and
  Codec2 ON CI passed; no new channel physics were added.

# ISSUE-0046: OFDM carrier layout and baseband frame prototype

- Status: closed
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

- [x] Add a carrier layout descriptor for OFDM prototype planning.
- [x] Preserve 1.9 kHz occupied bandwidth target metadata.
- [x] Distinguish data, pilot, and guard carrier placeholders.
- [x] Reject invalid carrier spacing, invalid occupied bandwidth, and zero
  carrier layouts.
- [x] Preserve future FEC and sync placeholder metadata.
- [x] Do not replace the ISSUE-0042 minimal QPSK runtime path with final OFDM.
- [x] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planned red tests: carrier layout creation, invalid spacing rejection, invalid
  bandwidth rejection, zero carrier rejection, and placeholder metadata round
  trip.
- 2026-06-29: Subagent A started with red tests for the Module 07 OFDM
  carrier-layout descriptor, invalid layout validation, and baseband-frame
  placeholder metadata propagation.
- 2026-06-29: Red test failed as expected on missing
  `f700f/ofdm_modem_waveform/carrier_layout.hpp`; green after adding Module 07
  descriptor, validation, baseband metadata helper, and contract test.

## Verification

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed, including 22/22 CTest.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with WSL-style
  `GIT_DIR`/`GIT_WORK_TREE` environment for this Windows worktree; 22/22 CTest
  passed, smoke sweep completed 27 rows, and 6 official Codec2 ON rows were
  validated. Transient `build-codec2`, generated sweep output, and generated
  report were cleaned after verification.

## Notes

This issue is a boundary/prototype issue only. Real 700F downselect remains
prohibited.

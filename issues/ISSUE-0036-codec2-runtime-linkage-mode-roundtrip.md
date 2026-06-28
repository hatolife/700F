# ISSUE-0036: Codec2 runtime linkage and Mode-boundary official FreeDV roundtrip

- Status: complete
- Priority: P0
- Scope: module:05,10,11,13
- Owner: Module 05 + Module 10 + Module 11 + Module 13
- Branch: feature/ISSUE-0036-codec2-runtime-linkage-mode-roundtrip
- Target version: v0.2.0 target
- Milestone: M2 follow-up
- Depends: ISSUE-0007, ISSUE-0012, ISSUE-0034
- Last updated: 2026-06-29

## Problem

ISSUE-0034 confirmed official Codec2/FreeDV 700D/700E source and upstream CLI
roundtrip evidence, but direct F700F runtime roundtrip remains blocked. The
guarded default build must keep `F700F_ENABLE_CODEC2=OFF` green while an optional
Codec2-enabled path is added.

## Acceptance Criteria

- [x] Add optional Codec2 runtime linkage without breaking default
  `F700F_ENABLE_CODEC2=OFF` CI.
- [x] Define ownership for `freedv_open`/`freedv_close` and adapter lifetime.
- [x] Implement audio amplitude conversion and buffering around `freedv_nin()`.
- [x] Provide Mode-boundary 700D/700E official waveform encode/decode smoke tests
  under a Codec2-enabled guard.
- [x] Keep official rows clearly distinct from emulator and surrogate rows.
- [ ] Keep real downselect blocked unless official runtime rows and waveform-capable
  700F rows both provide valid performance evidence.

## TDD Record

- 2026-06-29: Start with guarded tests in
  `tests/test_freedv_official_adapter.cpp`. Default `F700F_ENABLE_CODEC2=OFF`
  must continue to assert explicit unavailable behavior. Codec2-enabled builds
  must configure official 700D/700E modes, tolerate zero-length encode/decode
  without crashing, and complete at least one short official Mode-boundary
  encode/decode smoke path before any real downselect is allowed.
- 2026-06-29: Green verification reached `OFFICIAL_FREEDV_ROUNDTRIP_WORKING`.
  Default OFF CI passed. Codec2 ON build and 20/20 CTest passed. M2-F OFF smoke
  produced 21 completed / 6 skipped / 0 failed rows. M2-F ON smoke produced
  27 completed / 0 skipped / 0 failed rows, including 6 official FreeDV
  completed rows with explicit `official=true`, `codec2_available=true`, and
  `roundtrip_available=true` notes.

## Result

Implemented as an optional path. `F700F_ENABLE_CODEC2=OFF` remains the default and
does not build or link external Codec2. `F700F_ENABLE_CODEC2=ON` initializes the C
language, imports the pinned `external/codec2` source through CMake, links the
upstream `codec2` target into Module 05, opens separate TX/RX `struct freedv`
instances for official 700D/700E modes, and runs a minimal Mode-boundary
encode/decode path.

Real downselect remains blocked because 700F candidate rows are still synthetic
surrogates. Follow-up ISSUE-0037 tracks making Codec2 ON verification part of CI.

## Notes

- Created by M2-E Manager integration after ISSUE-0034 research completion.
- Push/PR not performed from this workspace.

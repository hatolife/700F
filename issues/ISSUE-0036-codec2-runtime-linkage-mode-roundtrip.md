# ISSUE-0036: Codec2 runtime linkage and Mode-boundary official FreeDV roundtrip

- Status: planned
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

- [ ] Add optional Codec2 runtime linkage without breaking default
  `F700F_ENABLE_CODEC2=OFF` CI.
- [ ] Define ownership for `freedv_open`/`freedv_close` and adapter lifetime.
- [ ] Implement audio amplitude conversion and buffering around `freedv_nin()`.
- [ ] Provide Mode-boundary 700D/700E official waveform encode/decode smoke tests
  under a Codec2-enabled guard.
- [ ] Keep official rows clearly distinct from emulator and surrogate rows.
- [ ] Keep real downselect blocked unless official runtime rows and waveform-capable
  700F rows both provide valid performance evidence.

## Notes

- Created by M2-E Manager integration after ISSUE-0034 research completion.
- Push/PR not performed from this workspace.

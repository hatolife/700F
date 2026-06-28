# ISSUE-0034: official FreeDV waveform roundtrip research/integration

## Status
closed

## Scope
module:05,10,dependency

## Priority
P1

## Owner
- Codex role: Module 05 + Module 10
- Recommended model: gpt-5.5-xhigh

## Branch
- Base branch: develop
- Work branch: research/ISSUE-0034-official-freedv-waveform-roundtrip
- Worktree: ../700F-ISSUE-0034
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Milestone: M2-D Waveform and Tooling Unblock

## Dependencies
- ISSUE-0007
- ISSUE-0012

## Task
Research and integrate official Codec2/FreeDV 700D/700E waveform roundtrip behind
explicit Codec2 availability guards.

## Acceptance criteria
- [x] Source-level FreeDV API findings are recorded.
- [x] Default `F700F_ENABLE_CODEC2=OFF` build remains green.
- [x] If blocked, blocked reason is documented.
- [x] Local CI passes for default build.

## TDD record
- 2026-06-29: Started with research-test-first discovery of the optional
  Codec2/FreeDV source/API surface before changing runtime behavior. Default
  `F700F_ENABLE_CODEC2=OFF` must remain the first green path.
- 2026-06-29: Initialized `external/codec2` and verified pinned commit
  `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`.
- 2026-06-29: `F700F_ENABLE_CODEC2=ON` header-gated adapter path passed:
  `bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"`,
  `bash -lc "cmake --build build-codec2-on --target test_freedv_official_adapter -j2"`,
  and `bash -lc "./build-codec2-on/test_freedv_official_adapter"`.
- 2026-06-29: Upstream Codec2 CLI path passed:
  `bash -lc "cmake -S external/codec2 -B build-codec2-upstream -DCMAKE_BUILD_TYPE=Debug -DLPCNET=OFF"`,
  `bash -lc "cmake --build build-codec2-upstream --target freedv_tx freedv_rx -j2"`,
  followed by manual 700D/700E `freedv_tx`/`freedv_rx` roundtrips using
  `external/codec2/raw/ve9qrp_10s.raw`.
- 2026-06-29: Default local CI passed with `bash -lc "./tools/run_ci_local.sh"`:
  20/20 CTest tests plus version and governance checks.

## Result
Completed as a guarded research/integration checkpoint. Source-level 700D/700E
API findings, manual upstream CLI roundtrip evidence, and the current
`F700F_ENABLE_CODEC2=ON` header-gated adapter state are documented in
`docs/specs/freedv_official_waveform_roundtrip.md` and
`docs/codec2_700d_700e_verification.md`.

Direct F700F runtime waveform roundtrip remains blocked by follow-up work:
optional Codec2 library linkage, `freedv_close` ownership, amplitude conversion,
and `freedv_nin()`-driven RX buffering through the Mode boundary.

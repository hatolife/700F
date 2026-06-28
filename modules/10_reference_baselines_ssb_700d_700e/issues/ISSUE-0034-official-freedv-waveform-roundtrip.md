# ISSUE-0034: official FreeDV waveform roundtrip research/integration

## Status
closed

## Scope
module:10

## Branch
- Work branch: research/ISSUE-0034-official-freedv-waveform-roundtrip

## Task
Record the official Codec2/FreeDV 700D/700E waveform roundtrip source anchors
and keep the Module 10 emulator/reference-baseline boundary distinct from
official Codec2 runtime behavior.

## Acceptance criteria
- [x] 700D/700E source-level API and mode findings are documented.
- [x] Official 700D/700E findings do not relabel descriptor-only emulators as
  official waveform evidence.
- [x] Default Codec2-disabled build remains green.
- [x] Blocked direct F700F runtime roundtrip state is documented.

## TDD record
- 2026-06-29: Research-test-first source pass confirmed
  `external/codec2/src/freedv_api.h` mode ids `FREEDV_MODE_700D == 7` and
  `FREEDV_MODE_700E == 13` at pinned commit
  `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`.
- 2026-06-29: Manual upstream CLI probe built `freedv_tx`/`freedv_rx` and
  roundtripped `external/codec2/raw/ve9qrp_10s.raw` through both 700D and 700E.
- 2026-06-29: `bash -lc "./tools/run_ci_local.sh"` passed with default
  `F700F_ENABLE_CODEC2=OFF`, 20/20 CTest tests, and version/governance checks.

## Result
Closed as a guarded research checkpoint. The official upstream waveform path is
verified manually, while F700F official modes remain guarded descriptors until a
follow-up links and drives the Codec2 runtime through the Mode boundary.

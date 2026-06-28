# ISSUE-0034: official FreeDV waveform roundtrip research/integration

## Status
closed

## Scope
module:05

## Branch
- Work branch: research/ISSUE-0034-official-freedv-waveform-roundtrip

## Task
Research and integrate Codec2/FreeDV waveform roundtrip behind explicit
availability guards.

## Acceptance criteria
- [x] API findings are documented.
- [x] Default Codec2-disabled build remains green.
- [x] Blocked state is recorded if integration cannot land.

## TDD record
- 2026-06-29: Added/kept guarded adapter contract coverage for the
  `F700F_ENABLE_CODEC2=OFF` unavailable path and the `F700F_ENABLE_CODEC2=ON`
  header-detected-but-not-linked ISSUE-0034 runtime guard.
- 2026-06-29: `bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"` passed after submodule initialization.
- 2026-06-29: `bash -lc "cmake --build build-codec2-on --target test_freedv_official_adapter -j2"` and `bash -lc "./build-codec2-on/test_freedv_official_adapter"` passed.
- 2026-06-29: `bash -lc "./tools/run_ci_local.sh"` passed with default
  `F700F_ENABLE_CODEC2=OFF`, 20/20 CTest tests, and version/governance checks.

## Result
Closed as a guarded research checkpoint. Direct F700F Codec2 runtime roundtrip
remains blocked by the missing `codec2` library link, `freedv_close` ownership,
sample amplitude conversion, and `freedv_nin()` RX buffering work.

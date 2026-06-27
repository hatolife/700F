# ISSUE-0012 FreeDV Official Baseline Adapter Handoff

## Summary

- Branch: `feature/ISSUE-0012-freedv-official-baseline-adapter`
- Worktree: `../f700f-wt-issue-0012`
- Subagent: M1-C subagent B
- Codec2 submodule: initialized at `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`

## Implemented

- Added Module 05 `f700f_codec_adapter` target.
- Added official descriptors for `freedv700d_official` and `freedv700e_official`.
- Official descriptors set `official_baseline=true` and `emulator=false`.
- Existing Module 10 emulator descriptors remain distinct with `emulator=true` and
  `official_baseline=false`.
- Default `F700F_ENABLE_CODEC2=OFF` builds keep the official descriptors discoverable and
  report explicit unavailable/skip status.
- `F700F_ENABLE_CODEC2=ON` includes the pinned Codec2 FreeDV API header only when the
  submodule header is present and compile-checks 700D/700E mode ids.

## Verification

- Red: `bash -lc "rm -rf build && cmake -S . -B build -DF700F_ENABLE_CODEC2=OFF && cmake --build build"`
  failed before implementation because `f700f/codec_adapter/freedv_official.hpp` was
  missing.
- Green: `bash ./tools/run_ci_local.sh` passed with 7 CTest tests, version check, and
  governance check.
- Guarded ON smoke:
  `bash -lc "cmake -S . -B build-codec2 -DF700F_ENABLE_CODEC2=ON && cmake --build build-codec2 --target test_freedv_official_adapter && ./build-codec2/test_freedv_official_adapter"`
  passed; generated `build-codec2/` was removed.

## Limitations

- Full Codec2 waveform encode/decode is not implemented.
- Sample-audio round trip through official 700D/700E remains deferred.
- Runtime calls in the enabled skeleton report a guarded runtime failure after successful
  configuration.

## Follow-up

- Add full Codec2 FreeDV runtime binding and sample-audio round trip validation.
- Decide whether official adapter registration should be composed into a larger default
  reference-baseline registry in a later integration issue.

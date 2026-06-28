# ISSUE-0034 Official FreeDV Waveform Roundtrip Handoff

## Current State

- Branch: `research/ISSUE-0034-official-freedv-waveform-roundtrip`
- Worktree: `C:/Users/user/Documents/700F-ISSUE-0034`
- Submodule: `external/codec2` initialized at
  `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`.
- Default behavior: `F700F_ENABLE_CODEC2=OFF` remains the required green path.
- Optional behavior: `F700F_ENABLE_CODEC2=ON` compiles the Module 05 guarded
  adapter against `freedv_api.h` and checks 700D/700E API ids, but it still does
  not link or call the upstream `codec2` runtime library.

## Research Results

- `FREEDV_MODE_700D == 7` and `FREEDV_MODE_700E == 13` are pinned in
  `external/codec2/src/freedv_api.h`.
- `freedv_open_advanced` dispatches 700D to OFDM voice mode `"700D"` and 700E to
  OFDM voice mode `"700E"` in `external/codec2/src/freedv_api.c`.
- TX entry points are `freedv_tx` for real short modem samples and
  `freedv_comptx` for complex `COMP` modem samples.
- RX entry points are `freedv_rx` for real short modem samples and
  `freedv_comprx` for complex `COMP` modem samples, with mandatory
  `freedv_nin()`-driven input sizing before each receive call.
- `external/codec2/src/ofdm_mode.c` keeps 700D defaults and applies 700E
  overrides for carrier count, symbol/CP timing, pilots, unique word bits, state
  machine, and LDPC codename.

## Verification

- `git submodule update --init --recursive`: passed.
- `git submodule status`: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962 external/codec2 (1.2.0-108-g310777b)`.
- `bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"`: passed.
- `bash -lc "cmake --build build-codec2-on --target test_freedv_official_adapter -j2"`: passed with clock-skew warnings only.
- `bash -lc "./build-codec2-on/test_freedv_official_adapter"`: passed.
- `bash -lc "cmake -S external/codec2 -B build-codec2-upstream -DCMAKE_BUILD_TYPE=Debug -DLPCNET=OFF"`: passed.
- `bash -lc "cmake --build build-codec2-upstream --target freedv_tx freedv_rx -j2"`: passed with clock-skew warnings only.
- Upstream 700D CLI roundtrip using `external/codec2/raw/ve9qrp_10s.raw`: passed,
  `frames decoded: 62  output speech samples: 79360`.
- Upstream 700E CLI roundtrip using `external/codec2/raw/ve9qrp_10s.raw`: passed,
  `frames decoded: 125  output speech samples: 80000`.

## Files To Read Next

- `docs/specs/freedv_official_waveform_roundtrip.md`
- `docs/codec2_700d_700e_verification.md`
- `modules/05_codec_adapter/src/freedv_official.cpp`
- `tests/test_freedv_official_adapter.cpp`

## Blocked Items

Direct F700F waveform roundtrip was not completed in ISSUE-0034. Remaining work:

- add or import upstream `codec2` as an optional CMake runtime target only when
  `F700F_ENABLE_CODEC2=ON`;
- own `struct freedv *` with `freedv_close`;
- decide whether F700F official modes should use real short samples, complex
  `COMP` samples, or both internally;
- define normalization between F700F float audio/IQ blocks and Codec2
  short/`COMP` amplitudes;
- implement RX buffering around `freedv_nin()`;
- add optional Codec2-on roundtrip tests that skip or are not configured when the
  submodule is missing.

## Merge Risk

Low for the default build: changes are docs plus guarded status/test wording.
Optional Codec2-on behavior remains non-performance evidence and should not
unblock real candidate downselect.

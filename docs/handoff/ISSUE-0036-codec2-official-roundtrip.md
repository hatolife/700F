# Handoff: ISSUE-0036 Codec2 Official FreeDV Roundtrip

- Branch: `feature/ISSUE-0036-codec2-runtime-linkage-mode-roundtrip`
- Decision: `OFFICIAL_FREEDV_ROUNDTRIP_WORKING`
- Codec2 pin: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Default OFF CI: passed
- Codec2 ON CTest: passed, 20/20
- Push/PR: not performed

## Implemented

- Added `F700F_CODEC2_ROOT` and optional Codec2 runtime linkage.
- OFF builds do not build or link Codec2.
- ON builds import the pinned `external/codec2` CMake project and link `codec2`.
- Module 05 official adapter owns TX/RX `struct freedv` handles and closes them
  with `freedv_close`.
- Official 700D/700E encode/decode smoke works through `IMode`.
- M2 campaign registration adds official factories only when Codec2 is available.
- OFF official rows skip with explicit `codec2_enabled=false` metadata.
- ON official rows complete with explicit `official=true` and
  `roundtrip_available=true` metadata.

## Artifacts

- OFF sweep: `artifacts/sweeps/m2f_smoke_codec2_off/m2f-codec2-off.json`
- ON sweep: `artifacts/sweeps/m2f_smoke_codec2_on/m2f-codec2-on.json`
- OFF comparison: `reports/20260629_031147_m2f_candidate_comparison_codec2_off.md`
- ON comparison: `reports/20260629_031147_m2f_candidate_comparison_codec2_on.md`

## Still Blocked

- Real 700F downselect remains blocked by synthetic 700F candidate rows.
- Codec2 ON path is not yet in CI; ISSUE-0037 tracks that.

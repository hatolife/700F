# M2-F Codec2 Official Roundtrip Report

## Decision

`OFFICIAL_FREEDV_ROUNDTRIP_WORKING`

ISSUE-0036 links the pinned Codec2 runtime behind `F700F_ENABLE_CODEC2=ON` and
connects official FreeDV 700D/700E modes through the F700F Mode boundary. Default
`F700F_ENABLE_CODEC2=OFF` remains green and does not build external Codec2.

## Verification

- Running commit during verification: feature branch after `3d0c976`
- Codec2 submodule commit: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Default OFF local CI: passed, 20/20 CTest, version check, governance check
- Codec2 ON build: passed with `cmake -S . -B build-codec2 -DF700F_ENABLE_CODEC2=ON`
- Codec2 ON CTest: passed, 20/20 CTest

## Smoke Results

| Build | Total | Completed | Skipped | Failed | Official completed | Official skipped |
|---|---:|---:|---:|---:|---:|---:|
| Codec2 OFF | 27 | 21 | 6 | 0 | 0 | 6 |
| Codec2 ON | 27 | 27 | 0 | 0 | 6 | 0 |

## Official Status

- 700D official: Codec2 ON Mode-boundary encode/decode smoke works.
- 700E official: Codec2 ON Mode-boundary encode/decode smoke works through the
  same adapter path and sweep completion.
- OFF skip reason: `official_freedv_codec2_unavailable`.
- ON completion note: `official_freedv_completed: official=true; codec2_enabled=true; codec2_available=true; roundtrip_available=true; not_emulator=true; not_surrogate=true`.

## Downselect

Real downselect remains blocked. Official baseline rows can now complete in Codec2
ON builds, but 700F candidate rows remain ISSUE-0032 synthetic surrogate rows.

## Follow-Up

- ISSUE-0037: add non-default Codec2 ON CI or documented regression workflow.
  Implemented as `tools/run_codec2_on_ci_local.sh` and the `linux-codec2-on`
  GitHub Actions job. Default Codec2-off CI remains submodule-independent.

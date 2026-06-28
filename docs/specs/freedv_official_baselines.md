# FreeDV Official Baselines v0.1.0

ISSUE-0012 adds official baseline descriptors for FreeDV 700D and 700E. These modes are
separate from the parameterized emulator descriptors added by ISSUE-0013.

## Mode ids

| Mode | F700F mode id | Codec2 mode id source |
| --- | --- | --- |
| FreeDV 700D official | `freedv700d_official` | `FREEDV_MODE_700D == 7` in `external/codec2/src/freedv_api.h` |
| FreeDV 700E official | `freedv700e_official` | `FREEDV_MODE_700E == 13` in `external/codec2/src/freedv_api.h` |

The official modes set `official_baseline=true` and `emulator=false`. The existing
`freedv700d_emulated` and `freedv700e_emulated` modes remain descriptor-only emulators
with `official_baseline=false` and `emulator=true`.

## Descriptor source anchors

Descriptor constants are derived from the pinned `external/codec2` submodule at
`310777b1c6f1af0bc7c72f5b32f80f6fd9136962`:

- `external/codec2/src/freedv_api.h` defines the FreeDV API mode ids.
- `external/codec2/src/ofdm_mode.c` defines 700D OFDM defaults and the 700E override
  branch used for carrier counts, symbol timing, CP timing, and LDPC codenames.
- `external/codec2/src/freedv_api.c` dispatches 700D and 700E FreeDV modes into OFDM
  voice modes `"700D"` and `"700E"`.

`docs/codec2_700d_700e_verification.md` records the full pin-source table. Web notes are
not authoritative for these constants unless a future issue links them back to pinned
source files and symbols.

`docs/specs/freedv_official_waveform_roundtrip.md` records the ISSUE-0034
source-level roundtrip API findings, the local upstream CLI 700D/700E manual probe,
and the ISSUE-0036 F700F Codec2 runtime linkage result.

## Availability behavior

Default builds keep `F700F_ENABLE_CODEC2=OFF`. In that configuration the official modes
are still discoverable, but their `implementation_status` contains an unavailable reason:
Codec2 support is skipped because `F700F_ENABLE_CODEC2=OFF`. Runtime `configure`,
`encode`, and `decode` calls fail gracefully with the same unavailable reason.

When `F700F_ENABLE_CODEC2=ON`, CMake requires `external/codec2/src/freedv_api.h` to be
present, imports the pinned upstream `codec2` target, and links Module 05 against the
runtime. The adapter opens official 700D and 700E FreeDV handles, uses complex TX/RX at
the Mode boundary, and reports official runtime smoke rows as available. This is still
not a 700F candidate downselect signal.

## Non-goals

ISSUE-0012 did not implement 700F optimization, copy or vendor Codec2 sources, or claim
bit-exact waveform parity with Codec2. ISSUE-0036 adds optional linked official runtime
smoke coverage, but full production waveform validation, recurring Codec2-on CI, and any
real 700F downselect remain follow-up work.

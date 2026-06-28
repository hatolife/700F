# Codec2 700D/700E Verification Notes

This document records the pinned Codec2/FreeDV source anchors for official baseline
verification. ISSUE-0007 pinned the source, and ISSUE-0012 uses these anchors for the
700D/700E official descriptor and availability adapter. The v0.1.0 adapter does not
implement bit-exact waveform encode/decode.

ISSUE-0034 adds official waveform roundtrip research. See
`docs/specs/freedv_official_waveform_roundtrip.md` for the detailed API map, the
local upstream CLI roundtrip probe, and the remaining F700F runtime blockers.

Pinned source:

- Submodule path: `external/codec2`
- Commit: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Upstream: `https://github.com/drowe67/codec2.git`

Authoritative anchors:

| Topic | Pinned source file | Symbol or location | Recorded value |
| --- | --- | --- | --- |
| FreeDV 700D mode id | `external/codec2/src/freedv_api.h` | `FREEDV_MODE_700D` | `7` |
| FreeDV 700E mode id | `external/codec2/src/freedv_api.h` | `FREEDV_MODE_700E` | `13` |
| Mode enable guards | `external/codec2/src/freedv_api.h` | `FREEDV_MODE_700D_EN`, `FREEDV_MODE_700E_EN` | Default to `FREEDV_MODE_EN_DEFAULT` |
| API mode dispatch | `external/codec2/src/freedv_api.c` | `freedv_open_advanced` | `FREEDV_MODE_700D` opens OFDM voice mode `"700D"`; `FREEDV_MODE_700E` opens OFDM voice mode `"700E"` |
| 700D modem defaults | `external/codec2/src/ofdm_mode.c` | `ofdm_init_mode`, default block for `"700D"` | `nc=17`, `np=1`, `ns=8`, `ts=0.018`, `tcp=0.002`, `tx_centre=1500.0`, `rx_centre=1500.0`, `fs=8000.0`, `txtbits=4`, `bps=2`, `codename="HRA_112_112"` |
| 700E modem overrides | `external/codec2/src/ofdm_mode.c` | `ofdm_init_mode`, `"700E"` branch | `ts=0.014`, `tcp=0.006`, `nc=21`, `ns=4`, `edge_pilots=0`, `nuwbits=12`, `txtbits=2`, `state_machine="voice2"`, `amp_est_mode=1`, `ftwindowwidth=80`, `codename="HRA_56_56"` |
| 700D LDPC code | `external/codec2/src/ldpc_codes.c` | `ldpc_codes[]`, `HRA_112_112` | Commented as short rate 1/2 code for FreeDV 700D |
| 700E LDPC code | `external/codec2/src/ldpc_codes.c` | `ldpc_codes[]`, `HRA_56_56` | Commented as short rate 1/2 code for FreeDV 700E |
| Upstream summary table | `external/codec2/README_freedv.md` | mode table rows for `700D` and `700E` | 700D: Codec2 700C, 17-carrier coherent OFDM/QPSK, RF BW 1000, raw 1900 bits/s, LDPC (224,112). 700E: Codec2 700C, 21-carrier coherent OFDM/QPSK, RF BW 1500, raw 3000 bits/s, LDPC (112,56). |
| Roundtrip API entry points | `external/codec2/src/freedv_api.h` | `freedv_open`, `freedv_close`, `freedv_tx`, `freedv_comptx`, `freedv_nin`, `freedv_rx`, `freedv_comprx` | First direct F700F runtime integration should own `struct freedv *`, prefer complex TX/RX for `ComplexBlock`, and buffer RX input according to `freedv_nin()`. |
| CLI roundtrip reference | `external/codec2/src/freedv_tx.c`, `external/codec2/src/freedv_rx.c` | 700D/700E mode parsing and raw sample loops | Local ISSUE-0034 probe built upstream `freedv_tx`/`freedv_rx` and roundtripped `external/codec2/raw/ve9qrp_10s.raw` for both 700D and 700E. |

Verification rule for future issues:

- Treat the pinned source files above as authoritative for adapter and baseline implementation.
- Use `README_freedv.md` as upstream documentation, not as the sole source for code constants when a C source or header symbol exists.
- Do not treat web-derived values as authoritative unless a future issue records the pinned source file, symbol/location, and commit hash that confirm them.
- ISSUE-0012 compile-checks the 700D/700E API ids when `F700F_ENABLE_CODEC2=ON` and keeps
  the default `F700F_ENABLE_CODEC2=OFF` path discoverable with an explicit unavailable
  status.
- ISSUE-0034 verified that the upstream pinned Codec2 CLI tools can roundtrip
  700D and 700E waveforms locally, but F700F still links only Codec2 headers in
  the optional enabled path. Direct F700F runtime roundtrip remains a follow-up
  requiring Codec2 library linkage, `freedv_close` ownership, float/short/`COMP`
  amplitude conversion, and `freedv_nin()`-driven RX buffering.

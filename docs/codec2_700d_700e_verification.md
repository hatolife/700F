# Codec2 700D/700E Verification Notes

This document records the pinned Codec2/FreeDV source anchors for later official baseline verification. It is source documentation only; ISSUE-0007 does not implement the official adapter, SSB, 700D, 700E, 700F, or later simulation issues.

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

Verification rule for future issues:

- Treat the pinned source files above as authoritative for adapter and baseline implementation.
- Use `README_freedv.md` as upstream documentation, not as the sole source for code constants when a C source or header symbol exists.
- Do not treat web-derived values as authoritative unless a future issue records the pinned source file, symbol/location, and commit hash that confirm them.

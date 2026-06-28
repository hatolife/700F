# FreeDV Official Waveform Roundtrip Research

ISSUE-0034 researched the pinned Codec2/FreeDV 700D and 700E API surface and
the then-current F700F guarded integration path. ISSUE-0036 follows that research
by linking the optional F700F Codec2-on build to the pinned runtime and proving
700D/700E official Mode-boundary smoke roundtrips. Default Codec2-off builds remain
the normal CI path.

## Source Pin

- Submodule path: `external/codec2`
- Commit: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Upstream: `https://github.com/drowe67/codec2.git`

## API Findings

| Topic | Pinned source | Symbol or line | ISSUE-0034 finding |
| --- | --- | --- | --- |
| 700D mode id | `external/codec2/src/freedv_api.h:52` | `FREEDV_MODE_700D` | Official API id is `7`. |
| 700E mode id | `external/codec2/src/freedv_api.h:53` | `FREEDV_MODE_700E` | Official API id is `13`. |
| Open/close API | `external/codec2/src/freedv_api.h:205-207` | `freedv_open_advanced`, `freedv_open`, `freedv_close` | Runtime integration needs an owning RAII wrapper around `struct freedv *`. |
| TX API | `external/codec2/src/freedv_api.h:211-212` | `freedv_tx`, `freedv_comptx` | F700F `ComplexBlock` should prefer `freedv_comptx`; the real-short CLI path remains useful for manual verification. |
| RX API | `external/codec2/src/freedv_api.h:226-231` | `freedv_nin`, `freedv_rx`, `freedv_comprx` | Decode must call `freedv_nin()` before each receive call and buffer exactly that many modem samples. |
| 700D/700E open dispatch | `external/codec2/src/freedv_api.c:112-148` | `freedv_open_advanced` | 700D opens OFDM voice mode `"700D"`; 700E opens OFDM voice mode `"700E"`. |
| TX sample contract | `external/codec2/src/freedv_api.c:303-314` | `freedv_tx` notes | Input speech is exactly `freedv_get_n_speech_samples()` shorts; output modem frame is exactly `freedv_get_n_nom_modem_samples()` samples. |
| Complex TX implementation | `external/codec2/src/freedv_api.c:353-392` | `freedv_comptx` | 700D/700E encode Codec2 frames into payload bits and call `freedv_comptx_ofdm`. |
| RX sample contract | `external/codec2/src/freedv_api.c:716-739` | `freedv_rx` notes | RX frame sizing is time-varying and `freedv_rx()` may return zero or variable speech samples. |
| Complex RX implementation | `external/codec2/src/freedv_api.c:807-830` | `freedv_comprx` | 700D/700E complex RX calls `freedv_comp_short_rx_ofdm` with the OFDM path. |
| Sample/query helpers | `external/codec2/src/freedv_api.c:1444-1463`, `external/codec2/src/freedv_api.c:1521-1538` | `freedv_get_*` | Adapter tests can query speech/modem sample rates and maximum frame sizes after `freedv_open`. |
| 700D OFDM defaults | `external/codec2/src/ofdm_mode.c:24-44` | `ofdm_init_mode` default block | 700D defaults include `nc=17`, `ns=8`, `ts=0.018`, `tcp=0.002`, `fs=8000`, `txtbits=4`, `bps=2`, `codename="HRA_112_112"`. |
| 700E OFDM overrides | `external/codec2/src/ofdm_mode.c:56-72` | `"700E"` branch | 700E overrides include `ts=0.014`, `tcp=0.006`, `nc=21`, `ns=4`, `edge_pilots=0`, `nuwbits=12`, `txtbits=2`, `state_machine="voice2"`, `codename="HRA_56_56"`. |
| Upstream CLI model | `external/codec2/src/freedv_tx.c:169-206`, `external/codec2/src/freedv_rx.c:206-268` | `freedv_tx`, `freedv_rx` apps | CLI tools show the official real-short waveform roundtrip sequence and option setup. |
| Codec2 CMake target | `external/codec2/src/CMakeLists.txt:245`, `external/codec2/src/CMakeLists.txt:259-262` | `codec2` | Upstream defines a real `codec2` library target with build-interface includes. ISSUE-0036 imports and links this target only when `F700F_ENABLE_CODEC2=ON`. |

## F700F Integration State

Default F700F builds keep `F700F_ENABLE_CODEC2=OFF`. In that path the official
FreeDV modes remain discoverable, but runtime `configure`, `encode`, and
`decode` fail with an explicit unavailable reason.

With `F700F_ENABLE_CODEC2=ON`, root `CMakeLists.txt` now validates
`external/codec2/src/freedv_api.h`, imports the upstream `codec2` target, exposes
`f700f_codec2_headers`, and links Module 05 through `f700f_codec2_runtime`.
The adapter compile-checks `FREEDV_MODE_700D == 7` and `FREEDV_MODE_700E == 13`,
owns separate TX/RX `struct freedv *` handles with `freedv_close`, calls
`freedv_comptx` and `freedv_comprx`, and buffers RX input around `freedv_nin()`.

ISSUE-0036 changes the F700F official adapter state from guarded skeleton to
optional linked runtime smoke coverage. Reports still distinguish this evidence
from real 700F candidate downselect data.

## Manual Verification

Submodule initialization succeeded:

```bash
git submodule update --init --recursive
git submodule status
# 310777b1c6f1af0bc7c72f5b32f80f6fd9136962 external/codec2 (1.2.0-108-g310777b)
```

F700F Codec2-enabled header path passed:

```bash
bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"
bash -lc "cmake --build build-codec2-on --target test_freedv_official_adapter -j2"
bash -lc "./build-codec2-on/test_freedv_official_adapter"
```

Upstream Codec2 CLI build and manual 700D/700E waveform roundtrips passed:

```bash
bash -lc "cmake -S external/codec2 -B build-codec2-upstream -DCMAKE_BUILD_TYPE=Debug -DLPCNET=OFF"
bash -lc "cmake --build build-codec2-upstream --target freedv_tx freedv_rx -j2"
bash -lc "build-codec2-upstream/src/freedv_tx 700D external/codec2/raw/ve9qrp_10s.raw build/issue-0034/700d_modem.raw"
bash -lc "build-codec2-upstream/src/freedv_rx 700D build/issue-0034/700d_modem.raw build/issue-0034/700d_speech.raw"
bash -lc "build-codec2-upstream/src/freedv_tx 700E external/codec2/raw/ve9qrp_10s.raw build/issue-0034/700e_modem.raw"
bash -lc "build-codec2-upstream/src/freedv_rx 700E build/issue-0034/700e_modem.raw build/issue-0034/700e_speech.raw"
```

Observed decode summaries:

| Mode | Modem bytes | Decoded speech bytes | RX summary |
| --- | ---: | ---: | --- |
| 700D | 158720 | 158720 | `frames decoded: 62  output speech samples: 79360` |
| 700E | 160000 | 160000 | `frames decoded: 125  output speech samples: 80000` |

Artifact hashes from the local manual probe:

| File | SHA-256 |
| --- | --- |
| `build/issue-0034/700d_modem.raw` | `a806d9277818578d4321305e36a5193f382928bd713a12d9444eb906474457f1` |
| `build/issue-0034/700d_speech.raw` | `cccbcc238582302a1d484500ce4d9160db0cfb69ee93c32c4845799730c92042` |
| `build/issue-0034/700e_modem.raw` | `87e541f9549aaeafe9ed00923cc0fe16e38bfc077d0e484b5b5e673a4e88dea6` |
| `build/issue-0034/700e_speech.raw` | `02511ef1b3c2fc31c2c8b2ec4f285d2606e21d60c5a08892ae5ae4d9fee4d748` |

## ISSUE-0036 Verification

F700F default Codec2-off CI passed:

```bash
bash ./tools/run_ci_local.sh
```

F700F Codec2-on runtime tests passed:

```bash
bash -lc "cmake -S . -B build-codec2 -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"
bash -lc "cmake --build build-codec2 -j2"
bash -lc "ctest --test-dir build-codec2 --output-on-failure"
```

M2-F smoke comparison status:

| Build | Completed | Skipped | Failed | Official 700D/700E rows |
| --- | ---: | ---: | ---: | --- |
| Codec2 off | 21 | 6 | 0 | Explicitly skipped as unavailable |
| Codec2 on | 27 | 0 | 0 | Completed with `official_freedv_completed` metadata |

## Remaining Runtime Work

ISSUE-0036 completes the first direct F700F official Mode-boundary smoke path.
ISSUE-0037 promotes that path into CI/local regression through
`tools/run_codec2_on_ci_local.sh` and the `linux-codec2-on` GitHub Actions job.
Remaining work is smaller and operational:

- broaden waveform validation beyond smoke-length and nonempty output checks;
- keep official baseline rows separate from real 700F candidate performance
  evidence until waveform-capable 700F rows exist.

Reports must continue to treat `freedv700d_official` and `freedv700e_official`
as official baseline evidence, not as F700F-produced candidate performance.

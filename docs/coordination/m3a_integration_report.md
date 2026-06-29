# M3-A Integration Report

## Scope

This report records the Manager integration of the M3-A low-risk groundwork
branches into `develop`.

## Integrated Issues

| Issue | Branch | Source commit | Merge commit | Result |
|---|---|---:|---:|---|
| ISSUE-0045 | `docs/ISSUE-0045-m3-planning-ofdm-fec-sync` | `fd0ed14` | `fbd0b15` | planning complete |
| ISSUE-0049 | `docs/ISSUE-0049-fec-placeholder-uep-policy-rfc` | `3b6d76a` | `2c8cef4` | merged |
| ISSUE-0046 | `feature/ISSUE-0046-ofdm-carrier-layout-baseband-frame` | `9bccc41` | `3ca00da` | merged |
| ISSUE-0047 | `feature/ISSUE-0047-occupied-bandwidth-validation` | `d39f226` | `f5f3180` | merged |
| ISSUE-0051 | `feature/ISSUE-0051-channel-impairment-regression` | `b9eb64d` | `7e095dd` | merged |

## Verification

- `bash ./tools/run_ci_local.sh` passed after ISSUE-0045, ISSUE-0049,
  ISSUE-0046, ISSUE-0047, and ISSUE-0051 integration.
- `bash ./tools/run_codec2_on_ci_local.sh` passed after ISSUE-0045,
  ISSUE-0049, ISSUE-0046, ISSUE-0047, and ISSUE-0051 integration.
- Final post-ISSUE-0051 default CTest count: 23/23.
- Final post-ISSUE-0051 Codec2 ON CTest count: 23/23.
- Final post-ISSUE-0051 Codec2 ON official smoke: 27/27 completed, 6 official
  FreeDV rows validated.
- Clock-skew warnings appeared during build and are non-blocking because build
  and tests passed.

## M3-A Smoke

- Codec2 OFF command:
  `./build/f700f-sweep --config configs/sweeps/m3_channel_impairment_smoke.toml --output-dir reports/sweeps/m3-channel-impairment-smoke-codec2-off`
- Codec2 OFF result: completed 28, skipped 8, failed 0.
- Codec2 ON command:
  `./build-codec2/f700f-sweep --config configs/sweeps/m3_channel_impairment_smoke.toml --output-dir reports/sweeps/m3-channel-impairment-smoke-codec2-on`
- Codec2 ON result: completed 36, skipped 0, failed 0.
- Reports:
  - `reports/20260629_133854_m3a_smoke_codec2_off.md`
  - `reports/20260629_133854_m3a_smoke_codec2_on.md`

## Boundary Status

- OFDM carrier layout status: M3-A boundary/prototype descriptor present; no
  final OFDM modem implemented.
- Occupied bandwidth validation status: deterministic simulation harness and
  report/artifact fields present; not a regulatory measurement.
- FEC placeholder status: RFC vocabulary and policy boundary present; no real
  FEC implemented.
- Channel regression status: short M3 smoke matrix covers identity, AWGN,
  frequency offset, and weak fading.
- Real downselect status: prohibited. 700F-A remains a limited prototype;
  700F-B/C remain surrogates.

## Next Work

Start ISSUE-0048, ISSUE-0050, and then ISSUE-0052/ISSUE-0053. ISSUE-0052 may be
used to formalize the smoke/report path now that M3-A artifacts exist. ISSUE-0053
must not authorize downselect unless later issues provide performance-valid real
modem evidence.

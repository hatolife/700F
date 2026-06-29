# Handoff: ISSUE-0045 Through ISSUE-0051 M3-A Integration

- Current branch: `develop`
- Merge target: `develop`
- Remote: `origin` configured
- Push/PR policy: direct branch/develop push; no PR required by repository
  instructions unless a human asks.

## Integrated State

ISSUE-0045 planning and the M3-A low-risk set are integrated into `develop`.

- ISSUE-0045: M3 planning, merge `fbd0b15`.
- ISSUE-0049: FEC placeholder and UEP policy RFC, merge `2c8cef4`.
- ISSUE-0046: OFDM carrier layout and baseband frame prototype, merge `3ca00da`.
- ISSUE-0047: occupied bandwidth validation harness, merge `f5f3180`.
- ISSUE-0051: channel impairment regression expansion, merge `7e095dd`.

## Verification

- Default Codec2 OFF local CI passed after each merge.
- Optional Codec2 ON local CI passed after each merge.
- Final default local CI after ISSUE-0051: 23/23 CTest, version check passed,
  governance check passed.
- Final Codec2 ON local CI after ISSUE-0051: 23/23 CTest, 27/27 official smoke
  rows completed, 6 official FreeDV rows validated.
- M3-A smoke:
  - Codec2 OFF: completed 28, skipped 8, failed 0.
  - Codec2 ON: completed 36, skipped 0, failed 0.

## Artifacts

- `docs/coordination/m3a_integration_report.md`
- `reports/20260629_133854_m3a_smoke_codec2_off.md`
- `reports/20260629_133854_m3a_smoke_codec2_on.md`
- `reports/sweeps/m3-channel-impairment-smoke-codec2-off/`
- `reports/sweeps/m3-channel-impairment-smoke-codec2-on/`

## Guardrails

- Real 700F downselect remains prohibited.
- 700F-A remains a limited `real_modem_prototype` with
  `not_final_modem=true`, `performance_validity=limited`, and
  `downselect_valid=false`.
- 700F-B and 700F-C remain surrogate rows.
- ISSUE-0046 did not implement final OFDM.
- ISSUE-0049 did not implement real FEC.
- ISSUE-0051 only expanded short deterministic regression coverage.

## Next Startable Issues

- ISSUE-0048: QPSK timing/sync placeholder.
- ISSUE-0050: frame interleaver placeholder.
- ISSUE-0052: M3 smoke campaign and report.
- ISSUE-0053: M3 audit after smoke/report work.

Recommended order: ISSUE-0048 and ISSUE-0050 may run in parallel if they keep
interfaces narrow; run ISSUE-0052 after the needed placeholders are in place;
run ISSUE-0053 last.

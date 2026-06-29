# Handoff: ISSUE-0045 M3 Planning For OFDM/FEC/Sync

- Date: 2026-06-29
- Branch: `docs/ISSUE-0045-m3-planning-ofdm-fec-sync`
- Merge target: `develop`
- Status: planning complete

## Summary

ISSUE-0045 defines M3 as a staged move from the ISSUE-0042 minimal QPSK-like
700F-A prototype toward OFDM/FEC/sync experimentation. It does not authorize a
final 700F modem, real downselect, or 700F-B/C real modem implementation.

## Outputs

- `docs/milestones/m3_ofdm_fec_sync.md`
- `docs/specs/700f_ofdm_fec_sync_plan.md`
- `docs/specs/700f_ofdm_carrier_layout_plan.md`
- `docs/specs/700f_fec_uep_policy_plan.md`
- `docs/coordination/m3_dispatch_plan.md`
- ISSUE-0046 through ISSUE-0053 root issue files
- module-local issue stubs for first M3 owners
- `reports/20260629_123435_issue0045_m3_planning.md`

## Verification

Pre-ISSUE-0045 Phase 0:

- `bash ./tools/run_ci_local.sh`: pass, 21/21 CTest, version check, governance
  check.
- `bash ./tools/run_codec2_on_ci_local.sh`: pass, 21/21 CTest, 27/27 smoke
  rows, 6 official FreeDV rows.

ISSUE-0045 branch:

- `bash ./tools/run_ci_local.sh`: pass, 21/21 CTest, version check, governance
  check.
- `bash ./tools/run_codec2_on_ci_local.sh`: pass, 21/21 CTest, 27/27 smoke
  rows, 6 official FreeDV rows.

## Next Startable Work

- ISSUE-0049: FEC placeholder and UEP policy RFC.
- ISSUE-0046: OFDM carrier layout and baseband frame prototype.
- ISSUE-0047: occupied bandwidth validation harness.
- ISSUE-0051: channel impairment regression expansion.

Preferred merge order: ISSUE-0049, ISSUE-0046, ISSUE-0047, ISSUE-0051.

## Blocked Work

- Real 700F candidate downselect.
- Final OFDM/FEC/sync implementation.
- 700F-B/C real modem upgrades.
- Performance claims against SSB, 700D, or 700E.

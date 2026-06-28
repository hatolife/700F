# Handoff: ISSUE-0041 Real 700F Modem RFC

- Branch: `docs/ISSUE-0041-real-700f-modem-architecture-rfc`
- Merge target: `develop`
- Depends: ISSUE-0040
- Current status: ready to merge
- Selected architecture: Option B then Option C

## What Changed

- Added `docs/specs/700f_real_modem_architecture_rfc.md`.
- Added `docs/specs/700f_modem_boundaries.md`.
- Defined the required real prototype metadata and boundary objects.
- Kept real downselect prohibited.

## Next Issues

- ISSUE-0042: 700F-A minimal QPSK baseband prototype.
- ISSUE-0043: modem frame and symbol protocol.
- ISSUE-0044: prototype modem metrics separation.
- ISSUE-0045: M3 planning for OFDM/FEC/sync.

## Verification

Run before merge:

- `bash ./tools/run_ci_local.sh`
- `bash ./tools/run_codec2_on_ci_local.sh`

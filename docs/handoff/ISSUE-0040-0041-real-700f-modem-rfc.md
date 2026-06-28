# Handoff: ISSUE-0040-0041 Real 700F Modem RFC

- Current branch: `docs/ISSUE-0041-real-700f-follow-up-issues`
- Latest branch commit before final handoff commit: `dfdd304`
- Merge target: `develop`
- Remote: `origin` configured
- Completed issues: ISSUE-0040, ISSUE-0041
- Created follow-up issues: ISSUE-0042, ISSUE-0043, ISSUE-0044, ISSUE-0045

## Integration State

- ISSUE-0040 is merged locally into `develop` as `882c961`.
- ISSUE-0041 RFC is merged locally into `develop` as `2fa3b51`.
- Follow-up issue expansion is on
  `docs/ISSUE-0041-real-700f-follow-up-issues` pending final merge.

## Local CI

- Default Codec2 OFF local CI: passed, 20/20 CTest, version check, governance
  check.
- Codec2 ON local CI: passed, 20/20 CTest, 27/27 smoke rows, 6 official FreeDV
  rows validated.
- Clock-skew warnings were observed and remain non-blocking per ISSUE-0035.

## Selected Architecture

Option B followed by Option C:

1. Minimal QPSK-like baseband prototype.
2. OFDM/FEC/sync expansion after boundaries stabilize.

## Next Startable Issues

| Issue | Recommended model | Branch | Owner |
|---|---|---|---|
| ISSUE-0043 | gpt-5.5-medium | `feature/ISSUE-0043-modem-frame-symbol-protocol` | Module 02 |
| ISSUE-0044 | gpt-5.5-medium | `feature/ISSUE-0044-prototype-modem-metrics-separation` | Module 12 |
| ISSUE-0042 | gpt-5.5-xhigh | `feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype` | Module 07 |
| ISSUE-0045 | gpt-5.5-xhigh | `docs/ISSUE-0045-m3-planning-ofdm-fec-sync` | Module 16 |

## Known Blockers

- ISSUE-0042 should wait for the minimal ISSUE-0043 source protocol boundary
  where practical.
- Real 700F downselect remains prohibited.
- M3 OFDM/FEC/sync implementation should not start from ISSUE-0045.

## Push And PR

Push the work branch and `develop` after final merge. PR is not required for this
local Manager integration flow unless a human explicitly asks for one.

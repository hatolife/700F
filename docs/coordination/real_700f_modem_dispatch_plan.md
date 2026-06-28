# Real 700F Modem Dispatch Plan

- Source decision: ISSUE-0041 real 700F modem architecture RFC
- Selected architecture: Option B followed by Option C
- Base branch: `develop`
- Merge target: `develop`
- Status: ready for follow-up assignment

| Issue | Recommended model | Branch | Worktree | Owner module | Preceding dependency |
|---|---|---|---|---|---|
| ISSUE-0043 modem frame and symbol protocol | gpt-5.5-medium | `feature/ISSUE-0043-modem-frame-symbol-protocol` | `../700F-ISSUE-0043` | Module 02 | ISSUE-0041 |
| ISSUE-0044 prototype modem metrics separation | gpt-5.5-medium | `feature/ISSUE-0044-prototype-modem-metrics-separation` | `../700F-ISSUE-0044` | Module 12 | ISSUE-0041; can start in parallel if it avoids source protocol coupling |
| ISSUE-0042 700F-A minimal QPSK baseband prototype | gpt-5.5-xhigh | `feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype` | `../700F-ISSUE-0042` | Module 07 | ISSUE-0043 preferred; ISSUE-0044 for final reporting/scoring |
| ISSUE-0045 M3 planning for OFDM/FEC/sync | gpt-5.5-xhigh | `docs/ISSUE-0045-m3-planning-ofdm-fec-sync` | `../700F-ISSUE-0045` | Module 16 | ISSUE-0042, ISSUE-0043, ISSUE-0044 |

## Dispatch Notes

- Start ISSUE-0043 first when possible because it stabilizes the protocol types.
- ISSUE-0044 may start in parallel if it focuses on artifact/report fields and
  avoids depending on unmerged source types.
- ISSUE-0042 should wait for at least the minimal ISSUE-0043 protocol boundary.
- ISSUE-0045 is planning-only and should not start implementation work.
- Real 700F downselect remains prohibited for all four issues.

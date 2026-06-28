# Real 700F Modem RFC Report

- Issue: ISSUE-0041
- Branch: docs/ISSUE-0041-real-700f-modem-architecture-rfc
- Base: `develop` after ISSUE-0040 merge
- Decision: RFC complete; follow-up issue expansion allowed
- Date: 2026-06-29

## Selected Architecture

Option B followed by Option C.

First, add a minimal QPSK-like baseband prototype for `freedv700f_a_balanced`
under `implementation_status = real_modem_prototype`. Then, after boundaries are
stable, expand toward OFDM/FEC/sync architecture in later issues and M3
planning.

## Design Output

- `docs/specs/700f_real_modem_architecture_rfc.md`
- `docs/specs/700f_modem_boundaries.md`

## Guardrails

- No real modem implementation in ISSUE-0041.
- No upgrade of 700F-B/C.
- No real candidate downselect.
- Keep Codec2 OFF and Codec2 ON CI green.
- Keep official 700D/700E regression smoke as baseline evidence.

## Follow-Up Direction

Create implementation and governance issues for:

- ISSUE-0042 minimal QPSK baseband prototype.
- ISSUE-0043 modem frame and symbol protocol.
- ISSUE-0044 prototype modem metrics separation.
- ISSUE-0045 M3 planning for OFDM/FEC/sync.

## Manager Handoff

- Final Manager handoff:
  `docs/handoff/ISSUE-0040-0041-real-700f-modem-rfc.md`
- Final Manager report:
  `reports/20260629_062002_real_700f_modem_rfc.md`
- Dispatch plan:
  `docs/coordination/real_700f_modem_dispatch_plan.md`

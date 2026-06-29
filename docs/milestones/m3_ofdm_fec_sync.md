# M3 OFDM/FEC/Sync Milestone

## Target

- Project target: `v0.3.0`
- Compatibility line: `3`
- Merge target: `develop`
- Entry condition: ISSUE-0042, ISSUE-0043, ISSUE-0044, and ISSUE-0045 merged.
- Downselect policy: real 700F downselect remains prohibited.

## Purpose

M3 moves 700F-A from the ISSUE-0042 minimal QPSK-like baseband prototype toward
an experimental OFDM/FEC/sync prototype in small, auditable steps.

M3 does not create final FreeDV 700F. It defines boundaries, measurements,
placeholder metadata, and regression coverage so later issues can add more real
modem behavior without confusing prototype diagnostics with final performance
evidence.

## Goals

- Move 700F-A toward OFDM/QPSK carrier-layout modeling.
- Make the 1.9 kHz occupied bandwidth target measurable in simulation reports.
- Add FEC placeholder and UEP policy boundaries that can later be replaced,
  while keeping real FEC and candidate downselect prohibited.
- Add sync placeholder boundaries for later timing/carrier recovery work.
- Expand channel impairment regression before deeper modem changes.
- Keep 700F-A `prototype=true`, `not_final_modem=true`, and
  `downselect_valid=false`.

## Non-Goals

- Final 700F modem.
- Performance claim over SSB, 700D, or 700E.
- 700D/700E compatibility.
- Final OFDM, FEC, synchronization, codec, or interleaver design.
- Real candidate downselect.
- 700F-B or 700F-C real modem implementation.

## Planned Root Issues

| Issue | Title | Phase | Owner | Branch |
|---|---|---|---|---|
| ISSUE-0045 | M3 planning for OFDM/FEC/sync | Planning | Manager + QA + Module 06 + Module 07 + Module 09 | `docs/ISSUE-0045-m3-planning-ofdm-fec-sync` |
| ISSUE-0046 | OFDM carrier layout and baseband frame prototype | M3-A | Module 07 + Module 02 + Module 11 + Module 13 | `feature/ISSUE-0046-ofdm-carrier-layout-baseband-frame` |
| ISSUE-0047 | occupied bandwidth validation harness | M3-A | Module 12 + Module 15 + Module 07 + Module 13 | `feature/ISSUE-0047-occupied-bandwidth-validation` |
| ISSUE-0048 | QPSK timing/sync placeholder | M3-B | Module 09 + Module 07 + Module 02 | `feature/ISSUE-0048-qpsk-timing-sync-placeholder` |
| ISSUE-0049 | FEC placeholder and UEP policy RFC | M3-A | Module 06 + Module 02 + Module 12 + Module 15 | `docs/ISSUE-0049-fec-placeholder-uep-policy-rfc` |
| ISSUE-0050 | frame interleaver placeholder | M3-B | Module 06 + Module 07 + Module 11 | `feature/ISSUE-0050-frame-interleaver-placeholder` |
| ISSUE-0051 | channel impairment regression expansion | M3-A | Module 08 + Module 13 + Module 11 + Module 15 | `feature/ISSUE-0051-channel-impairment-regression` |
| ISSUE-0052 | M3 smoke campaign and report | M3-B | Module 13 + Module 15 + Module 12 + QA | `feature/ISSUE-0052-m3-smoke-campaign-report` |
| ISSUE-0053 | M3 audit | M3 audit | Manager + QA | `docs/ISSUE-0053-m3-audit` |

## M3-A Startable Set

The first low-risk implementation/planning set was:

- ISSUE-0049: FEC placeholder and UEP policy RFC.
- ISSUE-0046: OFDM carrier layout and baseband frame prototype.
- ISSUE-0047: occupied bandwidth validation harness.
- ISSUE-0051: channel impairment regression expansion.

ISSUE-0049 should merge first because it defines no-FEC, placeholder-FEC,
future real-FEC, UEP bit-class, latency/interleaver, and reporting vocabulary.
ISSUE-0046 then adds the carrier-layout boundary. ISSUE-0047 adds measurement.
ISSUE-0051 connects the regression matrix last.

## M3-A Integration Status

M3-A is integrated into `develop`.

- ISSUE-0049 merged as `2c8cef4`.
- ISSUE-0046 merged as `3ca00da`.
- ISSUE-0047 merged as `f5f3180`.
- ISSUE-0051 merged as `7e095dd`.
- Default Codec2 OFF CI passed after each merge.
- Optional Codec2 ON CI passed after each merge.
- M3-A smoke passed with `configs/sweeps/m3_channel_impairment_smoke.toml`.
  Codec2 OFF completed 28 rows with 8 explicit official-baseline skips; Codec2
  ON completed 36 rows with no skips.
- 700F-A remains `real_modem_prototype`, `not_final_modem=true`, and
  `downselect_valid=false`.
- 700F-B and 700F-C remain surrogate rows.
- Real 700F downselect remains prohibited.

## Later Gates

ISSUE-0048, ISSUE-0050, ISSUE-0052, and ISSUE-0053 are the next startable
M3-B/audit set. ISSUE-0052 should run after ISSUE-0048 and ISSUE-0050 if it
needs those placeholder boundaries; ISSUE-0053 should run after ISSUE-0052.
None of them should authorize real downselect by default.

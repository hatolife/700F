# Module Spec

Module 07 owns prototype modem and waveform behavior for 700F candidate work.

## ISSUE-0042 Minimal QPSK Prototype

`freedv700f_a_balanced` uses the minimal QPSK-like baseband helper path exposed
through `include/f700f/candidate_profiles.hpp`. That path remains
prototype-only, downselect-invalid, and separate from final OFDM work.

## ISSUE-0046 OFDM Carrier Layout Prototype

`include/f700f/ofdm_modem_waveform/carrier_layout.hpp` exposes a metadata-only
OFDM carrier-layout boundary for M3 planning. The first descriptor records:

- 19 carrier placeholders at 100 Hz spacing.
- 1.9 kHz occupied-bandwidth target metadata.
- 15 data carrier placeholders, 2 pilot placeholders, and 2 guard placeholders.
- `fec_family=placeholder` and `sync_family=pilot_placeholder`.
- `prototype=true`, `not_final_modem=true`, and `downselect_valid=false`.

Validation rejects zero carriers, non-positive carrier spacing, non-positive
occupied-bandwidth targets, mismatched data/pilot/guard counts, and mismatched
carrier-role vectors. The baseband helper returns an empty `BasebandFrame` with
layout metadata and placeholder FEC/sync status only.

# API Contract

Module 07 owns the prototype modem/waveform behavior behind the 700F-A candidate
runtime.

ISSUE-0042 exposes a narrow prototype helper surface through
`include/f700f/candidate_profiles.hpp`:

- `make_700f_a_minimal_qpsk_input_frame()`
- `encode_700f_a_minimal_qpsk_symbols()`
- `modulate_700f_a_minimal_qpsk_baseband()`
- `demodulate_700f_a_minimal_qpsk_baseband()`

These helpers produce ISSUE-0043 `ModemInputFrame`, `SymbolBlock`,
`BasebandFrame`, and `ModemOutputFrame` values for
`freedv700f_a_balanced` only. They are prototype-only and must keep
`downselect_valid=false`, `performance_validity=limited`,
`sync_family=none`, `fec_family=none`, and `modem_family=minimal_qpsk`.

ISSUE-0046 adds Module 07-owned OFDM planning helpers through
`include/f700f/ofdm_modem_waveform/carrier_layout.hpp`:

- `make_700f_a_ofdm_carrier_layout_prototype()`
- `validate_ofdm_carrier_layout()`
- `count_carriers_with_role()`
- `make_700f_a_ofdm_baseband_frame_prototype()`

The descriptor records 19 carrier placeholders at 100 Hz spacing, a 1.9 kHz
occupied-bandwidth target, 15 data placeholders, 2 pilot placeholders, 2 guard
placeholders, `fec_family=placeholder`, and
`sync_family=pilot_placeholder`. The baseband helper returns an empty
`BasebandFrame` carrying metadata only; it does not generate OFDM samples and
does not replace the minimal QPSK runtime.

700F-B/C remain surrogate rows and are not upgraded by this contract.

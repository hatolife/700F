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

700F-B/C remain surrogate rows and are not upgraded by this contract.

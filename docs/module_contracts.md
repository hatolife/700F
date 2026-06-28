# Module Contracts

## M1 Mode Boundary

Module 02 owns the source-level data model for the v0.1.0 Mode interface:
`AudioBlock`, `ComplexBlock`, `BitBlock`, `SoftBitBlock`, `FrameStatus`,
`ModeDescriptor`, `ModeCapabilities`, `ModeRuntimeConfig`, `EncodeResult`, and
`DecodeResult`.

Module 02 also owns the ISSUE-0043 real-modem-prototype boundary value types:
`SampleRate`, `SymbolRate`, `CarrierLayout`, `TimingSyncStatus`, `FecStatus`,
`CodecStatus`, `PrototypeFrameStatus`, `ModemFrameDescriptor`,
`ModemInputFrame`, `SymbolBlock`, `BasebandFrame`, and `ModemOutputFrame`.
These are source-level containers and validation helpers, not a waveform or ABI
implementation.

Module 03 owns the registry/plugin-facing boundary for the same Mode concept:
`IMode`, `IModeFactory`, `ModeRegistry`, and the future C ABI plugin protocol described
in `docs/specs/plugin_protocol.md`.

Downstream modules may depend on descriptors, block containers, and interface contracts,
but ISSUE-0008 does not grant permission to implement specific SSB, FreeDV, Codec2, 700F,
or channel behavior.

## M2 700F Candidate Profile Boundary

ISSUE-0018 adds manager-owned 700F candidate profile helpers at
`include/f700f/candidate_profiles.hpp`. The helpers expose descriptor-only factories for
`freedv700f_a_balanced`, `freedv700f_b_robust`, and `freedv700f_c_quality` so registry,
metrics, sweep, and reporting code can select and snapshot candidates before modem
implementation exists.

Module 02 remains owner of the descriptor fields used by the profiles. Module 03 remains
owner of registry semantics. Modules 06, 07, 09, 11, 12, 13, and 15 may consume the
profile descriptors, but must treat `implementation_status = "surrogate"` as
not-real-modem, not-downselect-valid, non-performance evidence until their follow-up
issues replace the stub runtime with tested implementations.

700F candidates must not be labeled as official baselines. A profile with
`emulator = false` is still experimental unless `official_baseline = true`, which these
profiles deliberately do not set.

ISSUE-0032 adds append-only reporting/scoring fields for surrogate rows:
`not_real_modem`, `downselect_valid`, `not_downselect_valid`,
`performance_valid`, `surrogate_model_name`, `surrogate_model_version`, and
`surrogate_limitations`.

ISSUE-0043 keeps real-modem-prototype diagnostics separate from final scoring:
prototype frame status defaults to downselect-invalid and performance-limited
until a later audit authorizes final modem evidence.

ISSUE-0042 adds a narrow 700F-A helper surface in
`include/f700f/candidate_profiles.hpp` for creating a minimal QPSK-like
`ModemInputFrame`, `SymbolBlock`, `BasebandFrame`, and `ModemOutputFrame`.
Only `freedv700f_a_balanced` uses this real-modem-prototype path; 700F-B/C
remain surrogate rows. The helper surface is prototype-only and does not
authorize final FEC, OFDM, synchronization, Codec2 payloads, or real downselect.

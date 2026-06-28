# 700F Modem Boundaries

## Status

- Issue: ISSUE-0041
- Scope: protocol and architecture boundary
- Implementation status: RFC plus ISSUE-0043 source protocol boundary

The source-level C++20 boundary types are defined in `include/f700f/types.hpp`
and documented in `docs/specs/modem_frame_symbol_protocol.md`. ISSUE-0043 keeps
these as value-type containers and validation helpers only; it does not add real
waveform, FEC, synchronization, codec, or scoring behavior.

## Boundary Types

### ModemInputFrame

Represents the payload and run context entering a 700F modem prototype.

Required fields:

- `mode_id`
- `frame_index`
- `sample_rate_hz`
- `payload_bytes`
- `seed`
- `requested_modem_family`
- `requested_fec_family`
- `requested_sync_family`
- `requested_codec_family`

### SymbolBlock

Represents encoded modem symbols before sample generation.

Required fields:

- `modem_family`
- `symbol_rate_baud`
- `symbols`
- `bits_per_symbol`
- `fec_family`
- `sync_family`
- `limitations`

For the first follow-up, `symbols` may represent a minimal QPSK-like complex
symbol stream. It must not claim final OFDM behavior.

### BasebandFrame

Represents generated baseband samples and metadata.

Required fields:

- `sample_rate_hz`
- `occupied_bandwidth_hz`
- `center_frequency_hz`
- `samples`
- `symbol_rate_baud`
- `modem_family`
- `sync_family`
- `limitations`

### PrototypeFrameStatus

Represents prototype success, skip, or limitation state.

Required fields:

- `status`
- `reason`
- `implementation_status`
- `prototype`
- `not_final_modem`
- `downselect_valid`
- `performance_valid`
- `limitations`

Allowed `status` values:

- `completed`
- `skipped`
- `failed`
- `limited`

### ModemOutputFrame

Represents the decoded output and prototype diagnostics after receive-side
processing.

Required fields:

- `mode_id`
- `frame_index`
- `decoded_payload_bytes`
- `status`
- `symbol_error_count`
- `symbol_count`
- `prototype_metrics`
- `limitations`

## Family Fields

- `modem_family`: ISSUE-0042 uses `minimal_qpsk` for the first minimal
  QPSK-like baseband prototype.
- `fec_family`: first implementation may use `none` or `placeholder`.
- `sync_family`: first implementation may use `none`, `pilot_placeholder`, or
  `symbol_clock_placeholder`.
- `codec_family`: first implementation may use `synthetic` until codec payload
  integration is separately approved.

## Failure And Skip Semantics

Unavailable FEC, sync, or codec features must produce explicit status and
limitation metadata. Silent fallback to toy behavior is not allowed once a row is
marked `real_modem_prototype`.

## Metrics Connection

Metrics and reports may consume prototype diagnostics, but final scoring must
continue to treat real modem prototype rows as downselect-invalid until a later
audit changes the policy.

## ISSUE-0043 Source Mapping

`ModemFrameDescriptor` carries common frame metadata shared by input, symbol,
baseband, and output containers. `SampleRate` and `SymbolRate` expose minimal
positive-rate validation. `CarrierLayout`, `TimingSyncStatus`, `FecStatus`, and
`CodecStatus` are explicit placeholders so ISSUE-0042 can propagate modem
family metadata without silently implying final OFDM/FEC/sync/codec behavior.

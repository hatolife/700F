# 700F Real Modem Architecture RFC

## Status

- Issue: ISSUE-0041
- Status: RFC
- Target milestone: M2 real modem RFC
- Merge target: `develop`
- Decision: GO for follow-up issue expansion; no real modem implementation in
  this issue.

## Purpose

This RFC defines the step from the ISSUE-0039 `toy_audio_waveform` 700F-A
prototype toward a real 700F modem prototype. It creates a narrow architecture
gate so implementation can begin without confusing prototype readiness evidence
with final 700F performance or downselect evidence.

## Goals

- Define the first real modem prototype boundary for 700F-A.
- Separate modem frame, symbol, baseband, sync, FEC, codec, and metrics
  responsibilities.
- Preserve default Codec2 OFF CI and optional Codec2 ON regression behavior.
- Keep all 700F prototype rows explicitly non-final and downselect-invalid.
- Make follow-up work parallelizable without starting M3-level modem design too
  early.

## Non-Goals

- Do not implement a real modem in this RFC.
- Do not upgrade 700F-B or 700F-C.
- Do not perform real candidate downselect.
- Do not claim interoperability with upstream FreeDV modes.
- Do not finalize OFDM, FEC, synchronization, codec payload, or framing choices.

## Options

| Option | Summary | Benefit | Risk | Decision |
|---|---|---|---|---|
| A | Keep extending `toy_audio_waveform` | Smallest change | Keeps toy semantics too long and does not establish real modem boundaries | Reject |
| B | Add a minimal QPSK-like baseband prototype | Establishes symbols/baseband/status with bounded scope | Still not a final modem and needs strict metadata | Recommend first |
| C | Add OFDM/FEC/sync prototype architecture | Closer to plausible final modem work | Too large until frame/status boundaries are stable | Recommend after B |
| D | Jump to final downselect architecture | Fastest path to apparent decision | High risk of invalid conclusions and rework | Reject |

## Selected Architecture

Use Option B followed by Option C.

The next implementation should create a minimal QPSK-like baseband prototype for
`freedv700f_a_balanced` only. It should introduce explicit modem frame and symbol
boundaries, produce limited prototype metrics, and continue to mark all rows
downselect-invalid. Once those boundaries are stable, later work may expand into
OFDM/FEC/sync architecture under M3 planning.

## Required Metadata

Every real modem prototype row must expose equivalent metadata:

- `implementation_status = real_modem_prototype`
- `prototype = true`
- `not_final_modem = true`
- `downselect_valid = false`
- `not_downselect_valid = true`
- `performance_valid = limited`
- `modem_family`
- `fec_family`
- `sync_family`
- `codec_family`
- `limitations`

`performance_valid = limited` means the row may support prototype engineering
diagnostics. It does not mean the row is valid for final candidate ranking.

## Modem Families

The first allowed real prototype family is:

- `minimal_qpsk`

Later RFC or planning work may introduce:

- `ofdm_candidate`
- `ofdm_with_fec`
- `ofdm_with_sync`

## Boundary Summary

The detailed boundary objects are specified in
`docs/specs/700f_modem_boundaries.md`:

- `ModemInputFrame`
- `ModemOutputFrame`
- `SymbolBlock`
- `BasebandFrame`
- `PrototypeFrameStatus`

## Metrics And Reporting

Prototype modem metrics must remain separate from final scoring:

- `prototype_symbol_error_rate`
- `prototype_frame_status`
- `prototype_baseband_samples`
- `prototype_sync_status`
- `prototype_limitations`

Reports may show these diagnostics, but scoring must keep
`downselect_valid = false` and must not mix limited prototype diagnostics into
real performance ranking.

## Acceptance Gate For Implementation Follow-Up

The next implementation issue may start only if it:

- changes only `freedv700f_a_balanced`;
- preserves 700F-B/C surrogate state;
- keeps default Codec2 OFF CI green;
- keeps Codec2 ON official 700D/700E regression green;
- emits the required metadata in JSON/CSV/report paths;
- documents any skipped FEC/sync/codec features as explicit limitations.

## Downselect Policy

Real 700F downselect remains prohibited after this RFC. A later audit must
explicitly authorize downselect after the repository has performance-valid real
modem evidence, official baseline evidence, and report/scoring semantics that
separate final evidence from prototype diagnostics.

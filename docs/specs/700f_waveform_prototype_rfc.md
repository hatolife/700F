# 700F Waveform-Capable Prototype RFC

## Status

- Issue: ISSUE-0038
- Status: RFC
- Target milestone: M2 follow-up
- Merge target: `develop`

## Purpose

This RFC defines the first safe step from ISSUE-0032 700F surrogate behavior to
a waveform-capable 700F prototype. It intentionally limits the initial prototype
to one candidate so the pipeline, sweep, scoring, and reporting semantics can be
validated without implying that 700F modem design is final.

## Candidate Scope

The first eligible prototype is:

- `freedv700f_a_balanced`

The following candidates remain ISSUE-0032 surrogate rows until separate RFC or
implementation work explicitly upgrades them:

- `freedv700f_b_robust`
- `freedv700f_c_quality`

## Required Prototype Metadata

Every 700F-A prototype row must expose equivalent metadata:

- `implementation_status = waveform_prototype`
- `prototype = true`
- `not_final_modem = true`
- `waveform_capable = true`
- `downselect_valid = false`
- `not_downselect_valid = true`
- `performance_valid = false`
- `codec_family = synthetic`
- `fec_family = none`
- `modem_family = toy_qpsk` or `toy_audio_waveform`
- `prototype_limitations` explaining that the waveform path is experimental,
  synthetic, non-interoperable, and not valid for final performance comparison.

The exact serialized container may be result-artifact metrics, optional fields,
or report-specific metadata, but the fields must be visible in smoke artifacts
and comparison reports.

## Minimal Runtime Contract

The first implementation issue may use a toy waveform path. It does not need to
be interoperable with official FreeDV or Codec2. It must:

- be deterministic for fixed seed/config input;
- avoid crashing on zero-length or short input;
- return successful Mode-boundary encode/decode for the selected 700F-A path;
- keep Codec2 OFF builds independent of `external/codec2`;
- preserve Codec2 ON official FreeDV 700D/700E regression smoke.

If a minimal QPSK-shaped path is too large for the first implementation, a
`toy_audio_waveform` path is acceptable as long as it is explicitly marked as
experimental and non-final.

## Reporting And Scoring Guardrails

Prototype rows may count as waveform-capable readiness evidence. They must not
count as real 700F performance evidence.

Reports must keep these categories separate:

- official baseline evidence: official FreeDV 700D/700E Codec2 ON rows;
- non-official emulator evidence: 700D/700E emulator rows;
- 700F surrogate evidence: 700F-B and 700F-C until upgraded;
- 700F waveform prototype evidence: 700F-A after ISSUE-0039.

Scoring must preserve `real_performance_score` separation. Prototype rows may
receive readiness or prototype evidence labels, but they must keep
`performance_valid = false` and `downselect_valid = false`.

## Downselect Policy

Real 700F candidate downselect remains prohibited after this RFC and after the
first prototype implementation. A later audit may only consider downselect when
the repository has:

- waveform-capable 700F evidence with final or audit-approved modem semantics;
- official baseline evidence for the comparison set;
- report/scoring semantics that mark performance-valid rows separately from
  prototype or surrogate rows;
- an explicit issue and audit decision authorizing downselect.

## Follow-Up

ISSUE-0039 may implement the first minimal 700F-A waveform-capable prototype
under this RFC. ISSUE-0039 must not upgrade 700F-B or 700F-C and must not claim
real downselect readiness.

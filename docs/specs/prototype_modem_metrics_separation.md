# Prototype Modem Metrics Separation

## Status

- Issue: ISSUE-0044
- Scope: metrics, scoring, report classification
- Target milestone: M2 real modem prototype follow-up

## Purpose

Prototype modem diagnostics are useful engineering evidence, but they are not final
700F performance evidence. This spec defines append-only artifact fields and scoring
policy so waveform prototypes and future real modem prototypes cannot accidentally
enable candidate downselect.

## Implementation Classes

Rows and descriptor snapshots use `implementation_status` plus
`implementation_classification` to keep these cases distinct:

| Classification | Meaning | Performance validity | Downselect validity |
|---|---|---|---|
| `toy` | Local toy plumbing behavior | `invalid` | `invalid` |
| `surrogate` | Synthetic readiness model | `invalid` | `invalid` |
| `profile_only` | Candidate profile with no runtime evidence | `invalid` | `invalid` |
| `descriptor_only` | Descriptor/emulator metadata only | `invalid` | `invalid` |
| `waveform_prototype` | Non-final waveform-capable prototype | `limited` or `invalid` | `invalid` |
| `real_modem_prototype` | Limited modem prototype diagnostics | `limited` | `invalid` |
| `official` | Official baseline/runtime evidence | `valid` when completed | policy-dependent |

Existing boolean fields remain for compatibility:

- `performance_valid=false` for limited prototype rows;
- `performance_validity=limited` records that diagnostics exist but are not final
  performance evidence;
- `downselect_valid=false` and `downselect_validity=invalid` are required for every
  prototype row in the current repository state.

## Prototype Diagnostic Fields

`ResultArtifact` reserves top-level prototype diagnostic fields:

- `prototype_symbol_error_rate`
- `prototype_frame_status`
- `prototype_sync_status`
- `prototype_baseband_sample_count`
- `prototype_limitations`

Descriptor snapshots also carry:

- `prototype_warning`
- `prototype_limitations`
- `codec_family`
- `fec_family`
- `modem_family`

These fields are engineering diagnostics only. They must not be mixed into BER/FER,
dropout, latency, RF bandwidth, real performance score, or real downselect feasibility.

## Scoring Policy

The M2 scorer counts prototype rows separately:

- `prototype_count`
- `waveform_prototype_count`
- `real_modem_prototype_count`
- `prototype_diagnostic_count`

Completed prototype rows increment completed-run health counters, but they increment
`performance_invalid_count`, keep `completed_run_ratio=0.0` for the mode when no
performance-valid rows exist, and keep `real_performance_score=0.0`.

## Report Policy

Reports must show prototype diagnostics with warnings. They must also state that real
downselect is not possible when any surrogate, waveform prototype, real modem
prototype, profile-only, descriptor-only, emulated-surrogate, skipped, or failed row is
present.

`real_modem_prototype` specifically renders as limited diagnostic evidence with
`performance_validity=limited`, `downselect_valid=false`, and
`downselect_validity=invalid`.

## Current Guardrail

No current row may set `downselect_valid=true` for 700F prototype or surrogate evidence.
A later audit must explicitly authorize final performance-valid 700F evidence before
real candidate downselect can proceed.

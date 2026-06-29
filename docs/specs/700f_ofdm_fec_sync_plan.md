# 700F OFDM/FEC/Sync Plan

## Status

- Issue: ISSUE-0045
- Target milestone: M3
- Status: planning

## Boundary Direction

M3 extends the current 700F-A minimal QPSK-like prototype in three layers:

1. Carrier and frame metadata: OFDM layout descriptors, occupied bandwidth
   target metadata, and baseband-frame placeholders.
2. Replaceable protection/sync metadata: FEC placeholder, UEP policy, and sync
   placeholder statuses.
3. Regression and reporting: occupied bandwidth estimates, channel impairment
   smoke subsets, and explicit prototype warnings.

The runtime must continue to mark 700F-A as a prototype until a later audit
explicitly changes that policy.

## Required Guardrails

- `prototype=true`
- `not_final_modem=true`
- `downselect_valid=false`
- `downselect_validity=invalid`
- `performance_validity=limited` for prototype diagnostics
- no superiority claim over SSB, 700D, or 700E
- no 700F-B/C real modem upgrade without a separate issue

## M3-A Boundaries

ISSUE-0046 may add an OFDM carrier layout and baseband frame descriptor. It
should keep pilot, guard, FEC, and sync behavior as placeholders.

ISSUE-0047 may add a simulation-side occupied bandwidth estimate. The first
measurement can be FFT/energy-threshold based and should document limitations.

ISSUE-0049 defines no-FEC, placeholder-FEC, future real-FEC, and metadata-only
UEP policy vocabulary. It may describe important/less-important bit classes,
latency assumptions, report representation, and the relationship to the future
ISSUE-0050 interleaver placeholder. Real FEC, final coding-family selection,
and real downselect remain out of scope.

ISSUE-0051 may add short channel impairment regression configs covering AWGN,
frequency offset, and simple fading.

## Performance Evidence Policy

M3-A evidence is engineering evidence only. Reports may show bandwidth estimates
and prototype diagnostics, but scoring must continue to keep real downselect
disabled until ISSUE-0053 or a later audit explicitly changes the gate.

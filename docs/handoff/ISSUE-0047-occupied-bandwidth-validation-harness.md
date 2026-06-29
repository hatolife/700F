# Handoff: ISSUE-0047 Occupied Bandwidth Validation Harness

- Branch: `feature/ISSUE-0047-occupied-bandwidth-validation`
- Worktree: `C:/Users/user/Documents/700F-ISSUE-0047`
- Scope: Add a deterministic occupied bandwidth utility and propagate its
  diagnostic fields through result, sweep, scoring, and report paths.

## Implementation

- Module 12 owns `f700f/occupied_bandwidth.hpp` and the deterministic DFT
  energy-ratio measurement helper.
- Result artifacts now carry append-only occupied bandwidth estimate, low/high
  frequency, ratio, and status fields.
- Module 13 sweep rows serialize the fields and compute the diagnostic for
  completed real-modem-prototype channel-output samples.
- Module 15 loader/reporting paths preserve and render the diagnostic with a
  1.9 kHz target comparison.

## Guardrails

The harness is simulation-only and not regulatory-grade. It does not implement
final OFDM, FEC, sync, Codec2 payload integration, or real performance-valid
evidence. Real 700F downselect remains prohibited.

## Verification

Focused local verification passed for `occupied_bandwidth_contract`,
`metrics_result_artifacts`, `baseline_comparison_report_contract`, and
`sweep_runner_contract`.

Full Codec2 OFF and Codec2 ON local CI status is recorded in the root
ISSUE-0047 file for the branch.

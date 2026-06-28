# ISSUE-0038: 700F waveform-capable prototype RFC

- Status: complete
- Priority: P0
- Scope: module:02,07,11,12,13,15
- Owner: Module 02 + Module 07 + Module 13 + Module 15
- Branch: docs/ISSUE-0038-700f-waveform-prototype-rfc
- Target version: v0.2.0 target
- Milestone: M2 follow-up
- Depends: ISSUE-0032, ISSUE-0036, ISSUE-0037
- Last updated: 2026-06-29

## Problem

M2 now has repeatable official FreeDV 700D/700E evidence under optional Codec2 ON
CI, but 700F A/B/C remain surrogate rows. Before any 700F candidate can become a
waveform-capable prototype, the repository needs a narrow RFC that defines which
candidate may change, what metadata must be emitted, and what claims remain
prohibited.

## Acceptance Criteria

- [x] Define the 700F waveform-capable prototype boundary in
  `docs/specs/700f_waveform_prototype_rfc.md`.
- [x] Keep `freedv700f_b_robust` and `freedv700f_c_quality` as ISSUE-0032
  surrogate rows.
- [x] Mark `freedv700f_a_balanced` as the only candidate eligible for the first
  minimal waveform-capable prototype follow-up.
- [x] Require explicit metadata for prototype rows, including
  `implementation_status = waveform_prototype`, `prototype = true`,
  `not_final_modem = true`, `downselect_valid = false`,
  `performance_valid = false`, `waveform_capable = true`, `codec_family`,
  `fec_family`, `modem_family`, and limitations.
- [x] State that real 700F downselect remains prohibited until later audit
  evidence upgrades prototype rows to performance-valid rows.
- [x] Local Codec2 OFF and Codec2 ON CI pass.

## TDD record

- 2026-06-29: Started from `develop` at `b7afd93`. Default local CI passed with
  20/20 CTest, version check, and governance check. Codec2 ON local CI passed
  with 20/20 CTest and 27/27 official smoke rows.
- 2026-06-29: RFC-first work begins on
  `docs/ISSUE-0038-700f-waveform-prototype-rfc`; implementation is reserved for
  ISSUE-0039 after the RFC is integrated.
- 2026-06-29: RFC branch verification passed:
  `bash ./tools/run_ci_local.sh` completed 20/20 CTest plus version and
  governance checks; `bash ./tools/run_codec2_on_ci_local.sh` completed 20/20
  Codec2 ON CTest and 27/27 official smoke rows.

## Result

Completed. ISSUE-0038 defines the first waveform-capable prototype boundary for
700F-A only. 700F-B and 700F-C remain ISSUE-0032 surrogate rows. Prototype rows
must remain experimental, synthetic, performance-invalid, and downselect-invalid.
Real 700F candidate downselect remains prohibited.

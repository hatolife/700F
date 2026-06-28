# M2-H QPSK Prototype Integration Report

- Date: 2026-06-29
- Branch: `develop`
- Integrated issues: ISSUE-0043, ISSUE-0044, ISSUE-0042
- Merge target: `develop`
- Work branches:
  - `feature/ISSUE-0043-modem-frame-symbol-protocol`
  - `feature/ISSUE-0044-prototype-modem-metrics-separation`
  - `feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype`

## Integration Summary

ISSUE-0043 established the source-level modem frame, symbol, baseband, and
prototype status containers. ISSUE-0044 separated limited prototype diagnostics
from performance-valid scoring and downselect data. ISSUE-0042 then upgraded
only `freedv700f_a_balanced` to a minimal QPSK-like baseband prototype.

700F-A now reports:

- `implementation_status = real_modem_prototype`
- `prototype = true`
- `not_final_modem = true`
- `waveform_capable = true`
- `modem_family = minimal_qpsk`
- `codec_family = synthetic`
- `fec_family = none`
- `sync_family = none`
- `performance_validity = limited`
- `downselect_valid = false`
- `downselect_validity = invalid`

700F-B and 700F-C remain ISSUE-0032 surrogate rows. Real 700F downselect remains
prohibited.

## Verification

Default Codec2 OFF local CI:

- Command: `bash ./tools/run_ci_local.sh`
- Result: pass
- CTest: 21/21 passed
- Version check: pass
- Governance check: pass

Codec2 ON local CI:

- Command: `bash ./tools/run_codec2_on_ci_local.sh`
- Result: pass
- CTest: 21/21 passed
- Smoke rows: 27 completed, 0 skipped, 0 failed
- Official FreeDV rows: 6 validated

M2-H smoke/report, Codec2 OFF:

- Command: `./build/f700f-sweep --config configs/sweeps/m2_700f_candidate_smoke.toml --output-dir artifacts/sweeps/m2h_700f_qpsk_smoke --run-id m2h-700f-qpsk-smoke`
- Command: `./build/f700f-report --input artifacts/sweeps/m2h_700f_qpsk_smoke/m2h-700f-qpsk-smoke.json --output reports/20260629_080853_m2h_700f_qpsk_comparison.md`
- Result: pass
- Rows: 21 completed, 6 official FreeDV skips, 0 failed

M2-H smoke/report, Codec2 ON:

- Command: `./build-codec2/f700f-sweep --config configs/sweeps/m2_700f_candidate_smoke.toml --output-dir artifacts/sweeps/m2h_700f_qpsk_smoke_codec2_on --run-id m2h-700f-qpsk-smoke-codec2-on`
- Command: `./build-codec2/f700f-report --input artifacts/sweeps/m2h_700f_qpsk_smoke_codec2_on/m2h-700f-qpsk-smoke-codec2-on.json --output reports/20260629_080853_m2h_700f_qpsk_comparison_codec2_on.md`
- Result: pass
- Rows: 27 completed, 0 skipped, 0 failed
- Official FreeDV rows: 6 completed as optional Codec2 runtime rows

## Decision

M2-H may proceed to planning work for ISSUE-0045. It must not start real
downselect, OFDM implementation, FEC implementation, sync estimator
implementation, or final 700F modem claims until a later planning/audit issue
authorizes that scope.

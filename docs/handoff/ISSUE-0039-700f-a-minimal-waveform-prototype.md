# Handoff: ISSUE-0039 700F-A Minimal Waveform Prototype

- Branch: `feature/ISSUE-0039-700f-a-minimal-waveform-prototype`
- Merge target: `develop`
- Depends: ISSUE-0038
- Current status: ready to merge

## What Changed

- `freedv700f_a_balanced` now uses `implementation_status =
  waveform_prototype` and a deterministic toy audio waveform encode/decode path.
- `freedv700f_b_robust` and `freedv700f_c_quality` remain ISSUE-0032 surrogate
  rows.
- Sweep JSON/CSV now expose prototype guardrail fields:
  `prototype`, `not_final_modem`, `waveform_capable`, `codec_family`,
  `fec_family`, `modem_family`, and `prototype_limitations`.
- Report loading, scoring, and Markdown reporting keep prototype rows
  performance-invalid and downselect-invalid.

## Verification

- `ctest --test-dir build --output-on-failure`: passed 20/20.
- M2-G smoke: completed 21, skipped 6, failed 0.
- M2-G comparison report:
  `reports/20260629_052411_issue0039_m2g_comparison.md`.
- `bash ./tools/run_ci_local.sh`: passed.
- `bash ./tools/run_codec2_on_ci_local.sh`: passed, including 6 validated
  official FreeDV rows.

## Still Not Allowed

- No real 700F downselect.
- No final modem claims for 700F-A.
- No upgrade of 700F-B/C from surrogate.

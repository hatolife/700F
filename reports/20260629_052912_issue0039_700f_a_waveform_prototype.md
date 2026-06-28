# ISSUE-0039 700F-A Waveform Prototype Report

- Branch: `feature/ISSUE-0039-700f-a-minimal-waveform-prototype`
- Base: `develop` after ISSUE-0038 merge
- Audit time: 2026-06-29 05:29 JST
- Scope: 700F-A minimal waveform-capable prototype, B/C surrogate preservation,
  smoke/report/scoring guardrails

## Result

Success. `freedv700f_a_balanced` now runs a deterministic toy audio waveform
through the existing pipeline and emits guarded prototype metadata. 700F-B and
700F-C remain ISSUE-0032 surrogate rows.

Real 700F downselect remains prohibited.

## Commands

- `cmake --build build && ctest --test-dir build --output-on-failure`: passed,
  20/20 CTest.
- `./build/f700f-sweep --config configs/sweeps/m2_700f_candidate_smoke.toml --output-dir artifacts/sweeps/issue0039_m2g_smoke --run-id issue0039-m2g-smoke`:
  passed, completed 21, skipped 6, failed 0.
- `./build/f700f-report --input artifacts/sweeps/issue0039_m2g_smoke/issue0039-m2g-smoke.json --output reports/20260629_052411_issue0039_m2g_comparison.md`:
  passed.
- `bash ./tools/run_ci_local.sh`: passed, 20/20 CTest, version check, governance
  check.
- `bash ./tools/run_codec2_on_ci_local.sh`: passed, 20/20 Codec2 ON CTest,
  27/27 smoke rows, 6 official FreeDV rows validated.

## Evidence

- M2-G smoke JSON:
  `artifacts/sweeps/issue0039_m2g_smoke/issue0039-m2g-smoke.json`
- M2-G smoke CSV:
  `artifacts/sweeps/issue0039_m2g_smoke/issue0039-m2g-smoke.csv`
- M2-G comparison report:
  `reports/20260629_052411_issue0039_m2g_comparison.md`

## Guardrails

- 700F-A: `implementation_status=waveform_prototype`, `prototype=true`,
  `not_final_modem=true`, `waveform_capable=true`,
  `performance_valid=false`, `downselect_valid=false`.
- 700F-B/C: `implementation_status=surrogate`,
  `not_real_modem=true`, `performance_valid=false`,
  `downselect_valid=false`.
- Official FreeDV 700D/700E: default Codec2-OFF smoke skips remain explicit;
  Codec2 ON local regression completed and validated official rows.

## Follow-Up

No blocking follow-up is required for ISSUE-0039. Future work may replace the toy
waveform with a real candidate modem only under a new issue/RFC, with tests that
keep prototype/final performance evidence clearly separated.

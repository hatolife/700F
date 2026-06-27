# M1 Baseline Smoke Result

## Summary

- Branch: `develop`
- Executing commit: `434a572`
- Run time: 2026-06-28 00:39:29 +09:00
- Command: `bash -lc './build/test_sweep_runner && find build/test-artifacts -maxdepth 3 -type f | sort'`
- Config source: `configs/sweeps/m1_baseline_smoke.toml`
- Runtime path: C++ contract test helper `f700f::make_m1_baseline_smoke_sweep_config`; standalone sweep CLI and TOML parser are not implemented yet.
- Result: pass with limitations

## Artifact Paths

- Aggregate JSON: `build/test-artifacts/m1-baseline-smoke/m1-baseline-smoke.json`
- Aggregate CSV: `build/test-artifacts/m1-baseline-smoke/m1-baseline-smoke.csv`
- Completed run JSON: `build/test-artifacts/m1-baseline-smoke/runs/m1-baseline-smoke-000-dummy-mode-identity-seed-1.json`
- Completed run CSV: `build/test-artifacts/m1-baseline-smoke/runs/m1-baseline-smoke-000-dummy-mode-identity-seed-1.csv`

## Records

| Run | Status | Mode | Channel | Seed | Digest | Note |
|---|---|---|---|---|---|---|
| `m1-baseline-smoke-000-dummy-mode-identity-seed-1` | completed | `dummy.mode` | `identity` | 1 | `8bd382795b6178cb` | deterministic smoke path passed |
| `m1-baseline-smoke-001-freedv700d-official-identity-seed-1` | skipped | `freedv700d_official` | `identity` | 1 | - | `mode id not registered: freedv700d_official` |
| `m1-baseline-smoke-002-freedv700e-official-identity-seed-1` | skipped | `freedv700e_official` | `identity` | 1 | - | `mode id not registered: freedv700e_official` |

## M1 Audit Input

- The smoke runner produced stable JSON/CSV aggregate artifacts.
- The deterministic dummy-mode path completed successfully.
- Official FreeDV rows are represented as skips, not failures, in the default/Codec2-disabled smoke path.
- M1 audit can proceed with a likely GO_WITH_LIMITATIONS decision unless additional blockers are found.

## Follow-Up Needed

- Add a standalone sweep CLI.
- Add TOML parsing for sweep configs.
- Register or adapt official FreeDV 700D/700E execution paths when Codec2 waveform support is implemented.

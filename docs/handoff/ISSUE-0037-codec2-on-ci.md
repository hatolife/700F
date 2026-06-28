# Handoff: ISSUE-0037 Codec2 ON CI

- Branch: `chore/ISSUE-0037-codec2-on-ci-official-regression-smoke`
- Merge target: `develop`
- Codec2 pin: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Default OFF CI: passed before implementation; final merged CI rerun required.
- Codec2 ON local regression: passed with 20/20 CTest and 27/27 smoke rows.

## What Changed

- `tools/run_codec2_on_ci_local.sh` initializes `external/codec2`, configures
  `build-codec2` with `F700F_ENABLE_CODEC2=ON`, builds, runs CTest, runs the
  M2 smoke sweep, validates all official FreeDV rows, and generates a comparison
  report.
- `.github/workflows/ci.yml` now has a separate `linux-codec2-on` job.
- Default Linux and optional Windows jobs remain Codec2-off and do not initialize
  submodules.
- F700F libraries that are built after importing Codec2 are explicitly static to
  avoid upstream `BUILD_SHARED_LIBS` leaking into F700F target type.

## Evidence

- `bash ./tools/run_codec2_on_ci_local.sh`: passed.
- Codec2 ON CTest: 20/20 passed.
- Codec2 ON smoke: 27 completed, 0 skipped, 0 failed.
- Official FreeDV 700D rows: 3 completed.
- Official FreeDV 700E rows: 3 completed.
- Comparison report:
  `reports/20260629_035219_issue0037_codec2_on_comparison.md`.

## Push And PR

Remote publication is attempted after local merge. PR creation is optional and may
be skipped when `develop` is pushed directly after the local no-ff merge.

## Still Blocked

- Real 700F candidate downselect remains blocked.
- Windows Codec2-on CI is not required by ISSUE-0037 and remains a possible
  future enhancement.
- Broader official waveform/channel impairment regression remains future work.

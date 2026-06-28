# ISSUE-0037 Codec2 ON CI Report

- Report time: 2026-06-29 03:56:19 JST
- Branch: `chore/ISSUE-0037-codec2-on-ci-official-regression-smoke`
- Base: `develop` at `64bfb2c`
- Codec2 submodule: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`

## Implemented

- Added `tools/run_codec2_on_ci_local.sh`.
- Added GitHub Actions `linux-codec2-on` job.
- Kept `linux-codec2-off` and optional Windows job Codec2-off.
- Removed submodule dependency from default Codec2-off Actions checkout.
- Made F700F library targets affected by the Codec2 import explicitly static so
  upstream `BUILD_SHARED_LIBS` does not leak into F700F target type.

## Local Results

| Check | Result |
|---|---|
| Phase 0 `bash ./tools/run_ci_local.sh` | passed, 20/20 CTest |
| Codec2 ON wrapper | passed |
| Codec2 ON CTest | passed, 20/20 CTest |
| Codec2 ON smoke | 27 completed, 0 skipped, 0 failed |
| Official rows | 6 completed, metadata validated |
| Generated report | `reports/20260629_035219_issue0037_codec2_on_comparison.md` |

Clock-skew warnings appeared during local builds and remain non-blocking because
build, CTest, version, and governance checks passed.

## Push And PR

Push and PR are attempted after local commit and develop merge. Results are
recorded in the final Manager summary and handoff.

## Downselect

Real 700F downselect remains prohibited. Official baseline rows now have
regression coverage, but 700F candidate rows remain surrogate/non-performance
evidence.

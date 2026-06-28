# ISSUE-0037: Codec2 ON CI and official FreeDV regression smoke

- Status: complete
- Priority: P1
- Scope: module:01,05,10,13
- Owner: Module 01 + Module 05 + Module 10 + Module 13
- Branch: chore/ISSUE-0037-codec2-on-ci-official-regression-smoke
- Target version: v0.2.0 target
- Milestone: M2 follow-up
- Depends: ISSUE-0036
- Last updated: 2026-06-29

## Problem

ISSUE-0036 made Codec2-enabled official FreeDV 700D/700E roundtrip smoke work
locally, but default CI remains Codec2-off only. The optional ON path should be
covered by a separate CI job or documented manual regression workflow without
making submodule initialization mandatory for default builds.

## Acceptance Criteria

- [x] Add a non-default GitHub Actions job or documented local workflow for
  `F700F_ENABLE_CODEC2=ON`.
- [x] Initialize `external/codec2` only for the Codec2 ON job.
- [x] Run `ctest --test-dir build-codec2 --output-on-failure`.
- [x] Run a short official FreeDV 700D/700E smoke or equivalent focused test.
- [x] Keep default Codec2 OFF CI fast and independent of the submodule.

## Notes

- Created by ISSUE-0036 after local Codec2 ON build and 20/20 CTest passed.

## TDD record

- 2026-06-29: Phase 0 default local CI passed on `develop` before starting work:
  `bash ./tools/run_ci_local.sh` completed 20/20 CTest plus version and governance
  checks. Clock-skew warnings were observed and remain non-blocking per ISSUE-0035.
- 2026-06-29: Start with a local Codec2 ON regression wrapper requirement before
  changing CI YAML: the script must use strict bash mode, initialize
  `external/codec2`, keep `build-codec2` separate from default `build`, run
  `ctest --test-dir build-codec2 --output-on-failure`, and execute the official
  700D/700E smoke path.
- 2026-06-29: First Codec2 ON regression attempt exposed an ON-only CMake leak:
  upstream Codec2 defines `BUILD_SHARED_LIBS`, causing later F700F libraries to
  become shared and fail on non-PIC static dependencies. Fixed by preserving
  F700F target type and declaring affected F700F libraries `STATIC`.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed after the CMake
  fix: Codec2 submodule initialized at
  `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`, Codec2 ON CTest passed 20/20,
  smoke completed 27 rows with 0 skipped/failed, and 6 official 700D/700E rows
  were validated with `official_freedv_completed` metadata.

## Result

Completed. Default Codec2 OFF CI remains submodule-independent. Codec2 ON
coverage is available through `tools/run_codec2_on_ci_local.sh` and the separate
GitHub Actions `linux-codec2-on` job. The ON path initializes `external/codec2`,
configures `build-codec2`, runs CTest, runs the M2 smoke sweep, validates official
FreeDV 700D/700E completion metadata, and generates a comparison report.

Real downselect remains prohibited because 700F rows are still surrogate rows.

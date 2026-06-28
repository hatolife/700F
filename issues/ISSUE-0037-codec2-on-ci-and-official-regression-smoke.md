# ISSUE-0037: Codec2 ON CI and official FreeDV regression smoke

- Status: planned
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

- [ ] Add a non-default GitHub Actions job or documented local workflow for
  `F700F_ENABLE_CODEC2=ON`.
- [ ] Initialize `external/codec2` only for the Codec2 ON job.
- [ ] Run `ctest --test-dir build-codec2 --output-on-failure`.
- [ ] Run a short official FreeDV 700D/700E smoke or equivalent focused test.
- [ ] Keep default Codec2 OFF CI fast and independent of the submodule.

## Notes

- Created by ISSUE-0036 after local Codec2 ON build and 20/20 CTest passed.

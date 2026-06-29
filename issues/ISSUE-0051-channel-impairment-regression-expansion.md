# ISSUE-0051: channel impairment regression expansion

- Status: closed
- Priority: P0
- Scope: module:08,13,11,15
- Owner: Module 08 + Module 13 + Module 11 + Module 15
- Branch: feature/ISSUE-0051-channel-impairment-regression
- Target version: v0.3.0 target
- Milestone: M3-A low-risk OFDM/FEC/sync groundwork
- Depends: ISSUE-0045, ISSUE-0047
- Recommended model: gpt-5.5-medium
- Last updated: 2026-06-29

## Problem

M3 needs a short, deterministic channel impairment regression subset before
OFDM/FEC/sync work expands runtime behavior.

## Acceptance Criteria

- [x] Add or document an M3 smoke channel matrix.
- [x] Include AWGN, frequency offset, and simple fading conditions.
- [x] Keep seed handling deterministic.
- [x] Preserve explicit unavailable/skipped reasons for Codec2 OFF official rows.
- [x] Keep CI smoke short enough for routine regression.
- [x] Keep Codec2 OFF and Codec2 ON local CI green.

## TDD Record

- Planned red tests: matrix contains AWGN/frequency-offset/fading conditions,
  deterministic seeds, short smoke subset, and skip reason propagation.
- 2026-06-29: Started implementation on
  `feature/ISSUE-0051-channel-impairment-regression`. Red test target:
  `tests/test_sweep_runner.cpp` and `tests/test_sweep_config_loader.cpp`
  should require an M3 smoke channel matrix with AWGN, frequency offset, simple
  fading, deterministic seed `70051`, a short 4-condition subset, explicit
  Codec2 OFF skip reasons, and `downselect_valid=false` for 700F prototype
  rows.
- 2026-06-29: Red tests first failed on missing
  `make_m3_channel_impairment_smoke_sweep_config()`, then passed after adding
  the M3 helper and TOML configs. Focused checks passed:
  `sweep_runner_contract` and `sweep_config_loader_contract`.

## Verification

- 2026-06-29: `bash ./tools/run_ci_local.sh` passed: build, 21/21 CTest,
  version check, and governance check.
- 2026-06-29: `bash ./tools/run_codec2_on_ci_local.sh` passed with WSL
  `GIT_DIR`/`GIT_COMMON_DIR` worktree path override: 21/21 Codec2 ON CTest,
  27/27 smoke rows completed, and 6 official FreeDV rows validated. Transient
  Codec2 ON build/report artifacts were removed after verification.

## Result

Added the ISSUE-0051 M3 channel impairment smoke config/helper with four
conditions and seed `70051`. Default Codec2 OFF official FreeDV rows keep
explicit unavailable skip reasons; Codec2 ON official rows complete. 700F-A
remains `real_modem_prototype`, `not_final_modem=true`, and
`downselect_valid=false`; 700F-B/C remain surrogate rows.

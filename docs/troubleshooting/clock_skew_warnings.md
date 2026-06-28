# Clock-Skew Warnings in Local Build

## Symptom observed (ISSUE-0035)

- Running `bash ./tools/run_ci_local.sh` in this worktree reports repeated
  GNU Make `Clock skew detected` warnings, typically:
  - `CMakeFiles/.../compiler_depend.make` modification time 0.05s–2s in the future
  - `CMakeFiles/Makefile2` modification time 2s in the future
- All warnings are non-fatal; build and checks complete successfully:
  - `cmake --build` completes
  - `ctest` reports `100% tests passed, 0 tests failed out of 20`
  - version and governance checks pass.

## Likely causes checked

1. **WSL on mounted NTFS (`/mnt/c`) with drvfs/9p**
   - `mount` output shows the worktree is on `C:\` via `9p`:
     `C:\ /mnt/c 9p rw ...`
   - This path is known to produce coarse or out-of-band timestamp behavior for
     generated build files.
2. **Generated build metadata files**
   - Warnings repeatedly reference generated CMake artifacts such as
     `build/CMakeFiles/*/compiler_depend.make` and `build/CMakeFiles/Makefile2`.
   - This points to generation-order / timestamp-order effects during incremental
     `cmake --build` rather than source logic in the repository.
3. **Timestamp granularity / mixed clock reads**
   - `Make` reports fractional future times (`0.052s`, `0.91s`, `1.9s`), which is
     consistent with sub-second timestamp skew between WSL-side file operations
     and make's cached state.
4. **Less likely causes**
   - No risky timestamp-mangling logic was found in top-level CMake definitions.
     Build scripts use normal target/test wiring and do not intentionally rewrite file
     times.

## Practical handling

- Treat these warnings as informational on this environment until source/build timing is
  unified.
- Prefer a Linux-native working directory (for example, under `~/...`) for warning-free
  iterative rebuilds.
- On `/mnt/c`, keep rebuilds incremental and tolerate the warning when the full local CI
  suite remains green.
- If a deterministic local build is required without warning churn, run from a clean
  Linux filesystem clone and compare warning behavior.

## Timestamps reviewed

- Warning files are under `build/CMakeFiles/`, not versioned project sources.
- `CMakeLists.txt` in this worktree has older/moderate times relative to generated
  build metadata created during configure/build in the same run.

## Decision

No build-system behavior changes were introduced for this issue, because existing warnings
are reproducible but non-blocking and pass criteria are met.

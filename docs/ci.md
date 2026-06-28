# CI

Bootstrap M0 CI is intentionally fast and push-oriented.

## Policy

- GitHub PRs are not required for normal work; pushes to active work branches should trigger CI directly.
- Push coherent checkpoints frequently so failures are detected early.
- Do not wait idly for CI when independent implementation, review, cleanup, or documentation work can continue safely.
- Treat local CI as a quick pre-push guard and GitHub Actions as the canonical remote signal.

## Workflows

| Workflow | Trigger | Purpose |
|---|---|---|
| `.github/workflows/ci.yml` | `push`, `workflow_dispatch` | Required Linux Codec2-off CMake configure/build/CTest path, required Linux Codec2-on regression path, and optional Windows Codec2-off path (`continue-on-error`). |
| `.github/workflows/governance.yml` | `push`, `workflow_dispatch` | Lightweight documentation/version/index policy checks. |
| `.github/workflows/security.yml` | `push`, `workflow_dispatch`, weekly schedule | Early placeholder security checks for generated repository content. |

## Local equivalent

```bash
./tools/run_ci_local.sh
```

Optional Codec2-on regression:

```bash
./tools/run_codec2_on_ci_local.sh
```

## Local CI mapping

- `ci.yml` `linux-codec2-off` job ≈ `bash ./tools/run_ci_local.sh` for required
  default coverage. It does not initialize `external/codec2`; default CI must
  remain independent of the submodule.
- `ci.yml` `linux-codec2-on` job ≈ `bash ./tools/run_codec2_on_ci_local.sh`.
  It checks out submodules recursively, initializes `external/codec2`, configures
  `build-codec2` with `-DF700F_ENABLE_CODEC2=ON`, runs CTest, then runs the
  official 700D/700E smoke sweep and report generation.
- `ci.yml` Windows job is optional, Codec2-off, and intentionally non-blocking
  (`continue-on-error`), intended as early signal only.

The default local wrapper runs CMake configure/build, CTest, version validation,
and governance validation. The Codec2-on wrapper fails with an explicit message
if `external/codec2` cannot be initialized or `freedv_api.h` is missing.

## Codec2 ON Smoke

The Codec2-on regression validates that official FreeDV rows are not confused with
emulator or surrogate rows. It requires all six official smoke rows
(`freedv700d_official` and `freedv700e_official` over the smoke channel set) to
complete with `official_freedv_completed`, `codec2_available=true`,
`roundtrip_available=true`, `not_emulator=true`, and `not_surrogate=true`.

This is official baseline regression evidence only. Real 700F downselect remains
blocked until waveform-capable 700F candidate rows exist and are explicitly marked
as performance-valid.

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
| `.github/workflows/ci.yml` | `push`, `workflow_dispatch` | Required Linux CMake configure/build/CTest path plus optional Windows path (`continue-on-error`) that runs the same checks; both explicitly set `-DF700F_ENABLE_CODEC2=OFF` and run version/governance checks. |
| `.github/workflows/governance.yml` | `push`, `workflow_dispatch` | Lightweight documentation/version/index policy checks. |
| `.github/workflows/security.yml` | `push`, `workflow_dispatch`, weekly schedule | Early placeholder security checks for generated repository content. |

## Local equivalent

```bash
./tools/run_ci_local.sh
```

## Local CI mapping

- `ci.yml` Linux job ≈ `bash ./tools/run_ci_local.sh` for required coverage.
- `ci.yml` Windows job is optional and intentionally non-blocking (`continue-on-error`), intended as early signal only.
- `ci.yml` includes a submodule guard:
  - Fails fast with a clear message if any submodule is uninitialized.
  - Skips implicitly when no submodules are declared.

The local wrapper runs CMake configure/build, CTest, version validation, and governance validation.

# Manager Codex Instructions

Scope: entire repository.

## Operating model

- Work issue-first: update `issues/` before code changes.
- GitHub pull requests are not required for normal Codex work in this repository; use direct branch pushes unless a human explicitly asks for a PR.
- Push early and often to GitHub so CI runs frequently. Do not hold local work for a large batch when a coherent checkpoint can be pushed.
- Start CI as soon as practical, then continue useful non-conflicting work while CI runs; do not idle just waiting for CI unless the next step is genuinely blocked by the result.
- Prefer small commits/checkpoints that make CI failures easy to identify and revert.
- `develop` and `master` are protected by convention for release/integration discipline, but this project allows active work branches to be pushed freely.
- Manager-owned paths: root files, `docs/`, `include/f700f/`, `src/apps/`, `configs/`, `tests/integration/`, `tools/`, `.github/`.
- Module Codex may edit only its `modules/NN_<name>/` tree unless Manager approves an interface RFC.
- Public API/protocol changes require updates to `docs/protocol.md`, `docs/module_contracts.md`, and affected task docs.

## Local checks

Run these before pushing when practical, and rely on GitHub Actions for the canonical push-time signal:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
python3 tools/check_versions.py
python3 tools/check_governance.py
```

Or run the wrapper:

```bash
./tools/run_ci_local.sh
```

## Completion hygiene

- Record stubs and limitations in the relevant issue and docs.
- TODO comments must include an issue id.
- If CI fails after a push, create or update the relevant issue and keep moving on independent work while the fix is prepared.

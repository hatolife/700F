# ISSUE-0013 FreeDV Parameterized Emulator Handoff

## Summary

- Branch: `feature/ISSUE-0013-freedv-parameterized-emulator`
- Worktree: `C:/Users/user/Documents/f700f-wt-issue-0013`
- Scope: descriptor-only `freedv700d_emulated` and `freedv700e_emulated` Mode skeletons.
- Official baseline adapter: not implemented; ISSUE-0012 remains untouched.

## Implementation

- Added minimal ModeDescriptor fields for CP duration, audio bandwidth, carrier count,
  modulation family, pilot strategy, official baseline flag, emulator flag, and
  implementation status.
- Added Module 10 `f700f_freedv_emulator` library with descriptor accessors and
  `register_freedv_emulator_modes`.
- Encode/decode return `ok = false` with clear ISSUE-0013 "not implemented" errors.
- Documented placeholder descriptor assumptions in `docs/specs/freedv_emulator.md`,
  `docs/reference_emulator_validation.md`, and Module 07/10 limitations.

## TDD Evidence

- Red command:
  `bash -lc 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --target test_freedv_emulator && ctest --test-dir build --output-on-failure -R freedv_emulator_contract'`
- Red result: compile failed because `f700f/reference_baselines/freedv_emulator.hpp` was absent.
- Green command: same focused command passed after implementation.
- Local CI: `bash ./tools/run_ci_local.sh` passed with 3 tests plus version and governance checks.

## Limitations

- Descriptor values are initial assumptions and are not official FreeDV values.
- No Codec2 adapter, official FreeDV modem, FEC, interleaver, synchronizer, or
  high-fidelity OFDM runtime is implemented.
- Later high-quality emulator work can replace the runtime while preserving the Mode ids.

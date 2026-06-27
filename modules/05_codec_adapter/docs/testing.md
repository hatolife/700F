# Testing

Module 05 is covered by `test_freedv_official_adapter`.

The contract test verifies:

- official 700D/700E descriptors can be represented;
- official flags are true and emulator flags are false;
- official mode ids differ from the Module 10 emulator mode ids;
- `F700F_ENABLE_CODEC2=OFF` exposes clear unavailable status and runtime skip errors;
- `F700F_ENABLE_CODEC2=ON` smoke coverage is guarded by submodule availability and checks
  pinned Codec2 mode ids.

Default CI runs with Codec2 disabled through `bash ./tools/run_ci_local.sh`.

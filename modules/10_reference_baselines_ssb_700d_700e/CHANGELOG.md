# Changelog

## v0.2.0 - 2026-06-29
- Add ISSUE-0033 deterministic `emulated_surrogate` behavior for
  `freedv700d_emulated` and `freedv700e_emulated`, with explicit non-official,
  performance-invalid, and downselect-invalid report metadata.

## v0.2.0 - 2026-06-28
- Expose direct factory helpers for `freedv700d_emulated` and
  `freedv700e_emulated` so M2 sweep campaigns can register descriptor-only
  emulator rows without routing through a standalone registry.

## v0.1.0 - 2026-06-27
- Add placeholder SSB reference modes `ssb_standard_3k` and `ssb_narrow_1k9` with
  registry selection, descriptors, and simulation-pipeline identity-channel coverage.
- Add descriptor-only `freedv700d_emulated` and `freedv700e_emulated` Mode skeletons.

## v0.0.1 - 2026-06-27
- Bootstrap Reference Baselines: SSB, 700D, 700E module skeleton.

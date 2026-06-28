# Handoff: ISSUE-0038 700F Waveform Prototype RFC

- Branch: `docs/ISSUE-0038-700f-waveform-prototype-rfc`
- Merge target: `develop`
- Base commit: `b7afd93`
- RFC: `docs/specs/700f_waveform_prototype_rfc.md`

## What Changed

- Defined the first 700F waveform-capable prototype boundary.
- Authorized only `freedv700f_a_balanced` for the first follow-up prototype
  implementation.
- Kept `freedv700f_b_robust` and `freedv700f_c_quality` under ISSUE-0032
  surrogate semantics.
- Required prototype metadata that preserves experimental, synthetic,
  performance-invalid, and downselect-invalid labeling.

## Evidence

- `bash ./tools/run_ci_local.sh`: passed.
- Default CTest: 20/20 passed.
- Version check: passed.
- Governance check: passed.
- `bash ./tools/run_codec2_on_ci_local.sh`: passed.
- Codec2 ON CTest: 20/20 passed.
- Codec2 ON smoke: 27 completed, 0 skipped, 0 failed.
- Official FreeDV rows validated: 6.

## Next

Start ISSUE-0039 from `develop` after ISSUE-0038 is merged. ISSUE-0039 may make
only 700F-A waveform-capable. It must preserve B/C surrogate rows, keep Codec2
OFF/ON CI green, and keep real 700F downselect prohibited.

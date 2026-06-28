# Protocol

The M1 v0.1.0 source protocol introduces a common Mode boundary for local simulation
work. The normative v0.1.0 details are split by concern:

- `docs/specs/mode_interface.md` defines the C++20 `IMode`, `IModeFactory`, descriptor,
  runtime config, result, and registry surface.
- `docs/specs/data_protocol.md` defines the audio, complex, bit, soft-bit, and frame
  status value types exchanged at the boundary.
- `docs/specs/plugin_protocol.md` documents the future C ABI plugin relationship.
- `docs/specs/700f_candidate_architecture.md` defines the M2-A profile-only 700F
  candidate boundary and baseline-vs-candidate labeling rules.
- `docs/specs/700f_candidate_profiles.md` lists the three experimental 700F candidate
  profile descriptors introduced for ISSUE-0018.
- `docs/specs/scoring_model.md` defines the ISSUE-0024 interim M2 scoring and comparison
  policy over result artifacts and profile snapshots.

ISSUE-0008 is intentionally a protocol/interface issue. It does not implement 700F,
Codec2, SSB, FreeDV 700D/700E, or the channel model.

ISSUE-0018 introduces selectable 700F candidate profiles, but they remain
descriptor-only stubs with `implementation_status = "profile_only"` and do not encode
or decode a 700F waveform.

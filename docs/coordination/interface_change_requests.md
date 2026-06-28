# Interface Change Requests

## ISSUE-0008 Mode interface and protocol v0.1.0

- Status: accepted for local implementation by Manager-scoped ISSUE-0008 dispatch.
- Branch: `feature/ISSUE-0008-mode-interface-v0-1`
- Scope: public C++20 Mode interface, data protocol docs, future C ABI plugin protocol
  docs, focused contract tests, and module 02/03 v0.1.0 manifest updates.
- Non-goals: 700F implementation, Codec2 adapter, SSB/700D/700E mode implementation,
  ISSUE-0009+ simulation core, and channel model.

## ISSUE-0024 M2 scoring model v0.2.0

- Status: accepted for local implementation by Manager-scoped ISSUE-0024 dispatch.
- Branch: `feature/ISSUE-0024-scoring-model`
- Scope: Module 12 `f700f/scoring_model.hpp` public C++ scoring API, append-only
  `ModeDescriptorSnapshot` fields for official/emulator/profile-only status, focused
  scoring contract tests, and scoring policy documentation.
- Non-goals: redefining ISSUE-0019 through ISSUE-0022, implementing ASR/STOI/ESTOI
  metrics, implementing 700F waveforms, or changing the existing required metrics
  artifact fields.

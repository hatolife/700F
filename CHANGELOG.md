# Changelog

## v0.2.0 - 2026-06-28
- Integrate M2-B scoring/channel/sweep/report path and record smoke/comparison reports
  with all campaign modes skipped pending mode-factory registration.
- Add ISSUE-0026 M2 700F candidate smoke/full sweep campaign configs over the M2
  channel matrices with stable ordering and skip-capable unavailable modes.
- Add ISSUE-0027 M2 baseline comparison report generation with deterministic Markdown
  sections, skipped/unavailable baseline status, profile-only 700F candidate display,
  and timestamped filename support.
- Add ISSUE-0024 interim M2 scoring model and comparison policy for result artifacts,
  official-unavailable records, audio-only N/A BER/FER, and 700F profile snapshots.
- Add ISSUE-0025 M2 smoke/full channel condition matrices and sweep-runner validation
  for duplicate condition ids, malformed SNR/frequency offsets, and empty seed lists.
- Record M2-A ISSUE-0018 integration and split requested scoring/channel/sweep/report
  work into follow-up issues because existing ISSUE-0019 through ISSUE-0022 are already
  assigned to FEC/OFDM/sync/AI triage.
- Define ISSUE-0018 M2-A 700F candidate architecture/profile boundary with
  descriptor-only profile stubs for balanced, robust, and quality candidates.
- Add profile registry/factory helpers and contract tests for descriptor retrieval,
  profile-only encode/decode status, metrics snapshots, and sweep/report visibility.

## v0.1.0 - 2026-06-27
- Add M2 700F candidate exploration planning issues and dispatch plan after M1 GO_WITH_LIMITATIONS.
- Complete M1 audit with GO_WITH_LIMITATIONS and mark project/M1-touched module manifests as v0.1.0.
- Record M1 baseline smoke output: one completed dummy-mode identity run and skipped official FreeDV 700D/700E entries under Codec2-disabled/default registration.
- Integrate M1-C: SSB reference modes, guarded official FreeDV descriptors, and initial sweep runner.
- Add ISSUE-0012 official FreeDV 700D/700E descriptors and guarded Codec2 availability
  adapter with default OFF unavailable status.
- Add ISSUE-0015 initial sweep runner with stable mode/channel/seed expansion,
  skipped-mode records, failed-run continuation, aggregate JSON/CSV output, and
  M1 baseline smoke schema.
- Integrate M1-B: metrics schema, channel models, parameterized emulator skeletons, and deterministic simulation pipeline.
- Add ISSUE-0013 descriptor-only FreeDV 700D/700E parameterized emulator skeletons with
  explicit emulator-vs-official baseline flags.
- Add the ISSUE-0009 deterministic simulation core with dummy mode, identity channel, dummy metric, stage statuses, and JSON/CSV summaries.
- Integrate M1-A: governance policy, GitHub Actions CI, common Mode interface/protocol, and Codec2 submodule pin.
- Add Linux-required and Windows-optional GitHub Actions CI documentation for ISSUE-0005.
- Document project/module governance, reporting, issue, and version policies for ISSUE-0006.
- Define the common Mode interface and source-level data protocol for ISSUE-0008.
- Define metrics/result artifact JSON and CSV format with stable required columns and optional extension points (ISSUE-0014).
- Document the future C ABI plugin protocol relationship without implementing plugin loading.
- Add focused Mode interface contract tests and update module 02/03 version metadata.
- Pin Codec2/FreeDV as `external/codec2` submodule and document 700D/700E verification anchors for ISSUE-0007.
- Implement M1 baseline channel model primitives (AWGN, frequency offset, simple gain fading, chain order) in module 08 for ISSUE-0010.

## v0.0.1 - 2026-06-27
- Plan M1 Baseline Simulation with root/module issues and target v0.1.0 manifest entries.
- Add fast push-oriented GitHub Actions CI/governance/security workflows and update AGENTS.md to avoid PR requirements.
- Bootstrap repository governance, documentation, module skeleton, and minimal C++20 build.

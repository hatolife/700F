# 700F Candidate Architecture RFC v0.2.0

ISSUE-0018 defines the M2-A boundary for experimental 700F candidate modes. It does
not define a final FreeDV 700F modem, waveform, codec adapter, sync loop, or FEC
scheme.

## Candidate Boundary

700F candidates are profile-first Mode descriptors. A candidate profile records the
intended RF bandwidth target, audio passband, latency class, robustness class, FEC
policy, modem family, and comparison goal before implementation branches begin.

In this phase, candidate runtimes must:

- register through `ModeRegistry` using stable `ModeId` values;
- expose `ModeDescriptor` metadata for metrics, sweep, and reporting snapshots;
- set `official_baseline = false`;
- set `emulator = false`;
- set `implementation_status = "profile_only"`;
- return `ok == false` from encode/decode with an error that includes the mode id,
  `ISSUE-0018`, and `profile_only`.

This keeps candidates selectable by infrastructure without allowing accidental claims
that a 700F modem implementation exists.

## Baseline And Candidate Labels

Result artifacts and reports must keep these categories distinct:

| Category | Example ids | `official_baseline` | `emulator` | Meaning |
|---|---|---:|---:|---|
| SSB references | `ssb_standard_3k`, `ssb_narrow_1k9` | false | true | Local reference-mode approximations for comparison. |
| Parameterized FreeDV emulators | `freedv700d_emulated`, `freedv700e_emulated` | false | true | Descriptor-only 700D/700E approximations, not official waveforms. |
| Official FreeDV adapters | `freedv700d_official`, `freedv700e_official` | true | false | Guarded Codec2/FreeDV adapter descriptors; runtime availability depends on Codec2 integration. |
| 700F candidates | `freedv700f_a_balanced`, `freedv700f_b_robust`, `freedv700f_c_quality` | false | false | Experimental M2 700F candidate profiles; profile-only until later implementation issues land. |

Reports must not collapse `emulator == false` into an official claim. Official status
comes only from `official_baseline == true`.

## Data Contracts

The source-level contract remains the existing Mode interface:

- Module 02 owns `ModeDescriptor`, runtime config, block, and result value types.
- Module 03 owns `ModeRegistry`, `IMode`, and `IModeFactory`.
- Candidate profile helpers live at `include/f700f/candidate_profiles.hpp`.

No new wire protocol is required for ISSUE-0018. Metrics/reporting consumers use the
existing `ModeDescriptorSnapshot` fields. The snapshot does not currently include
`official_baseline`, `emulator`, or `implementation_status`; reports that need those
distinctions must retain the full descriptor or add append-only snapshot fields in a
future issue.

## M2 Implementation Split

ISSUE-0018 ends at profile descriptors and profile-only runtime stubs.

The following issues own implementation work:

- ISSUE-0019: FEC/UEP prototype and tests against the profile FEC-policy names.
- ISSUE-0020: OFDM waveform candidate prototype and tests against the profile modem
  and bandwidth contract.
- ISSUE-0021: sync/impairment estimator prototype and tests against the waveform
  candidate once ISSUE-0020 exposes a minimal contract.
- ISSUE-0022: AI-assisted sweep triage and report ranking over completed, skipped, and
  failed candidate records.

## Red-Test Plan For Follow-Up Issues

ISSUE-0019 should first add tests that fail until each profile maps to a concrete FEC
policy object: medium for A, stronger for B, lighter for C, with UEP assumptions
documented.

ISSUE-0020 should first add tests that fail until the OFDM candidate can produce a
deterministic nonempty complex block under the 1.9 kHz occupied-bandwidth target and
preserve descriptor identity through encode/decode attempts.

ISSUE-0021 should first add tests that fail until the sync/impairment estimator reports
frequency offset and fading/sync status without changing profile labels or pretending to
decode unsupported waveforms.

ISSUE-0022 should first add tests that fail until sweep triage can include profile-only
failed candidate records, skipped unavailable baselines, and completed reference runs in
one stable report without ranking failed profile-only stubs as performance evidence.

## Limitations

ISSUE-0018 candidate profiles are not interoperable FreeDV modes. They do not encode,
decode, synchronize, apply FEC, or validate Codec2 parameters. The occupied-bandwidth
target is a descriptor-level design target until waveform tests in ISSUE-0020 measure an
actual signal.

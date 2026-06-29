# Project Issues

## Scope

- Root tracker for cross-module or cross-functional work.
- Module-local tracking remains in `modules/<NN>_<name>/issues/README.md`.

## Project issue policy

- Keep this index as the source of truth for project issue ownership and status.
- Issue-first rule: issue files must exist in the issue directory before implementation starts.
- Branch-first rule: branch/worktree entries in each issue file are source of truth for the issue owner.
- TDD-first rule: update issue `TDD record` before implementation for non-planning issues.

| ID | Status | Priority | Scope | Title | Owner | Branch | Target version | Milestone | Depends | Last updated |
|---|---|---|---|---|---|---|---|---|---|---|
| ISSUE-0001 | closed | P0 | project | Bootstrap repository | Manager | work | v0.0.1 | M0 | - | 2026-06-27 |
| ISSUE-0002 | closed | P0 | project | Fast push CI policy and workflows | Manager | work | v0.0.1 | M0 | ISSUE-0001 | 2026-06-27 |
| ISSUE-0003 | closed | P0 | project | M1 planning issue expansion | Manager | docs/ISSUE-0003-m1-planning | v0.1.0 | M1 | ISSUE-0001, ISSUE-0002 | 2026-06-27 |
| ISSUE-0004 | closed | P0 | qa | M0 audit and M1 go decision | Manager / QA | docs/ISSUE-0004-m0-audit | v0.1.0 | M1 | ISSUE-0001, ISSUE-0002 | 2026-06-27 |
| ISSUE-0005 | closed | P0 | project | GitHub Actions CI introduction | Module 01 / Manager | docs/ISSUE-0005-github-actions-ci | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0006 | closed | P0 | docs | Docs issues reports governance structure | Manager + all modules | docs/ISSUE-0006-governance-structure | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0007 | closed | P0 | dependency | external codec2 submodule pin | Manager + Module 05 + Module 10 | research/ISSUE-0007-codec2-submodule-pin | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0008 | closed | P0 | protocol | Common Mode interface and data protocol v0.1.0 | Module 02 + Module 03 + Manager | feature/ISSUE-0008-mode-interface-v0-1 | v0.1.0 | M1 | ISSUE-0004 | 2026-06-27 |
| ISSUE-0009 | closed | P0 | simulation | Deterministic simulation core | Module 11 + Module 02 + Module 12 | feature/ISSUE-0009-deterministic-simulation-core | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0010 | closed | P0 | module:08 | Channel model v0.1.0 | Module 08 | feature/ISSUE-0010-channel-model-v0-1 | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0011 | closed | P0 | module:10 | SSB reference emulator | Module 10 + Module 04 | feature/ISSUE-0011-ssb-reference-emulator | v0.1.0 | M1 | ISSUE-0008, ISSUE-0010 | 2026-06-28 |
| ISSUE-0012 | closed | P0 | module:05,10 | FreeDV 700D 700E official baseline adapter | Module 05 + Module 10 | feature/ISSUE-0012-freedv-official-baseline-adapter | v0.1.0 | M1 | ISSUE-0007, ISSUE-0008 | 2026-06-28 |
| ISSUE-0013 | closed | P1 | module:07,10 | 700D 700E parameterized emulator skeleton | Module 07 + Module 10 | feature/ISSUE-0013-700d-700e-parameterized-emulator | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0014 | closed | P0 | module:12 | Metrics and result artifact format | Module 12 + Module 15 | feature/ISSUE-0014-metrics-artifact-format | v0.1.0 | M1 | ISSUE-0008 | 2026-06-27 |
| ISSUE-0015 | closed | P0 | module:13 | Sweep runner v0.1.0 | Module 13 + Module 11 + Module 15 | feature/ISSUE-0015-sweep-runner-v0-1 | v0.1.0 | M1 | ISSUE-0009, ISSUE-0014 | 2026-06-28 |
| ISSUE-0016 | closed | P0 | qa | M1 audit report | Manager + QA | docs/ISSUE-0016-m1-audit-report | v0.1.0 | M1 | ISSUE-0004 through ISSUE-0015 | 2026-06-28 |
| ISSUE-0017 | closed | P0 | project | M2 planning issue expansion | Manager | docs/ISSUE-0017-m2-planning | v0.2.0 target | M2 | ISSUE-0016 | 2026-06-28 |
| ISSUE-0018 | closed | P0 | architecture | 700F candidate architecture RFC | Module 02 + Manager | docs/ISSUE-0018-700f-candidate-architecture-rfc | v0.2.0 | M2 | ISSUE-0017 | 2026-06-28 |
| ISSUE-0019 | open | P0 | module:06 | FEC UEP engine prototype | Module 06 + Module 02 | feature/ISSUE-0019-fec-uep-prototype | v0.2.0 target | M2 | ISSUE-0018 | 2026-06-28 |
| ISSUE-0020 | open | P0 | module:07 | OFDM waveform candidate prototype | Module 07 + Module 09 | feature/ISSUE-0020-ofdm-waveform-candidate | v0.2.0 target | M2 | ISSUE-0018 | 2026-06-28 |
| ISSUE-0021 | open | P0 | module:09 | sync and impairment estimator prototype | Module 09 + Module 07 | feature/ISSUE-0021-sync-impairment-estimator | v0.2.0 target | M2 | ISSUE-0018, ISSUE-0020 | 2026-06-28 |
| ISSUE-0022 | open | P1 | module:14 | AI-assisted sweep triage | Module 14 + Module 13 | research/ISSUE-0022-ai-sweep-triage | v0.2.0 target | M2 | ISSUE-0015, ISSUE-0018 | 2026-06-28 |
| ISSUE-0023 | closed | P0 | qa | M2 limited audit and candidate downselect gate | Manager + QA | docs/ISSUE-0023-m2-audit-downselect | v0.2.0 | M2 | ISSUE-0018 through ISSUE-0028 | 2026-06-28 |
| ISSUE-0024 | closed | P0 | module:12 | M2 scoring model and comparison policy | Module 12 + Module 13 | feature/ISSUE-0024-scoring-model | v0.2.0 | M2 | ISSUE-0018 | 2026-06-28 |
| ISSUE-0025 | closed | P0 | module:08 | M2 expanded channel condition matrix | Module 08 + Module 13 | feature/ISSUE-0025-expanded-channel-matrix | v0.2.0 | M2 | ISSUE-0018, ISSUE-0010 | 2026-06-28 |
| ISSUE-0026 | closed | P0 | module:13 | M2 sweep campaign runner config | Module 13 + Module 11 | feature/ISSUE-0026-m2-sweep-campaign-config | v0.2.0 | M2 | ISSUE-0018, ISSUE-0025 | 2026-06-28 |
| ISSUE-0027 | closed | P0 | module:15 | M2 baseline comparison report generation | Module 15 + Module 12 | feature/ISSUE-0027-baseline-comparison-report | v0.2.0 | M2 | ISSUE-0018, ISSUE-0024, ISSUE-0026 | 2026-06-28 |
| ISSUE-0028 | closed | P0 | module:13,10 | M2-C campaign mode registry bridge | Module 13 + Module 10 + Module 02 | feature/ISSUE-0028-m2-campaign-mode-registry | v0.2.0 | M2 | ISSUE-0018, ISSUE-0026 | 2026-06-28 |
| ISSUE-0029 | closed | P0 | module:10,11,15 | SSB audio export | Module 10 + Module 11 + Module 15 | feature/ISSUE-0029-ssb-audio-export | v0.2.0 | M2-D | ISSUE-0023, ISSUE-0028 | 2026-06-29 |
| ISSUE-0030 | closed | P0 | module:13 | sweep CLI and config loader | Module 13 + Manager | feature/ISSUE-0030-sweep-cli-config-loader | v0.2.0 | M2-D | ISSUE-0023, ISSUE-0028 | 2026-06-29 |
| ISSUE-0031 | closed | P0 | module:15,12 | report JSON loader and CLI | Module 15 + Module 12 | feature/ISSUE-0031-report-json-loader-cli | v0.2.0 | M2-D | ISSUE-0023, ISSUE-0027, ISSUE-0028 | 2026-06-29 |
| ISSUE-0032 | closed | P0 | module:02,06,07,09,11,13 | 700F candidate minimal behavior bridge | Module 02 + candidate modules | feature/ISSUE-0032-700f-candidate-minimal-behavior | v0.2.0 target | M2-D | ISSUE-0029, ISSUE-0030, ISSUE-0031 | 2026-06-29 |
| ISSUE-0033 | closed | P0 | module:10,11,13 | 700D/700E emulator minimal behavior bridge | Module 10 + Module 11 + Module 13 | feature/ISSUE-0033-700d-700e-emulator-minimal-behavior | v0.2.0 | M2-D | ISSUE-0029, ISSUE-0030, ISSUE-0031 | 2026-06-29 |
| ISSUE-0034 | closed | P1 | module:05,10 | official FreeDV waveform roundtrip research/integration | Module 05 + Module 10 | research/ISSUE-0034-official-freedv-waveform-roundtrip | v0.2.0 target | M2-D | ISSUE-0007, ISSUE-0012 | 2026-06-29 |
| ISSUE-0035 | closed | P2 | module:01 | clock-skew warning investigation | Module 01 + Manager | chore/ISSUE-0035-clock-skew-warning-investigation | v0.2.0 | M2-D | ISSUE-0023 | 2026-06-29 |
| ISSUE-0036 | closed | P0 | module:05,10,11,13 | Codec2 runtime linkage and Mode-boundary official FreeDV roundtrip | Module 05 + Module 10 + Module 11 + Module 13 | feature/ISSUE-0036-codec2-runtime-linkage-mode-roundtrip | v0.2.0 target | M2 follow-up | ISSUE-0007, ISSUE-0012, ISSUE-0034 | 2026-06-29 |
| ISSUE-0037 | closed | P1 | module:01,05,10,13 | Codec2-on CI and official regression smoke | Module 01 + Module 05 + Module 10 + Module 13 | chore/ISSUE-0037-codec2-on-ci-official-regression-smoke | v0.2.0 target | M2 follow-up | ISSUE-0036 | 2026-06-29 |
| ISSUE-0038 | closed | P0 | module:02,07,11,12,13,15 | 700F waveform-capable prototype RFC | Module 02 + Module 07 + Module 13 + Module 15 | docs/ISSUE-0038-700f-waveform-prototype-rfc | v0.2.0 target | M2 follow-up | ISSUE-0032, ISSUE-0036, ISSUE-0037 | 2026-06-29 |
| ISSUE-0039 | closed | P0 | module:02,07,11,12,13,15 | 700F-A minimal waveform-capable prototype | Module 02 + Module 07 + Module 13 + Module 15 | feature/ISSUE-0039-700f-a-minimal-waveform-prototype | v0.2.0 target | M2-G waveform prototype | ISSUE-0038 | 2026-06-29 |
| ISSUE-0040 | closed | P0 | qa,project | M2 audit update after 700F-A prototype | Manager + QA | chore/ISSUE-0040-m2-audit-update-after-700f-a-prototype | v0.2.0 target | M2 real modem RFC gate | ISSUE-0037, ISSUE-0038, ISSUE-0039 | 2026-06-29 |
| ISSUE-0041 | closed | P0 | module:02,07,11,12,13,15 | real 700F modem architecture RFC | Module 02 + Module 07 + Module 13 + Module 15 | docs/ISSUE-0041-real-700f-modem-architecture-rfc | v0.2.0 target | M2 real modem RFC | ISSUE-0040 | 2026-06-29 |
| ISSUE-0042 | closed | P0 | module:07,02,11,12,13,15 | 700F-A minimal QPSK baseband prototype | Module 07 + Module 02 + Module 13 + Module 15 | feature/ISSUE-0042-700f-a-minimal-qpsk-baseband-prototype | v0.2.0 target | M2 real modem prototype follow-up | ISSUE-0041, ISSUE-0043 | 2026-06-29 |
| ISSUE-0043 | closed | P0 | module:02,07,11,13 | modem frame and symbol protocol | Module 02 + Module 07 + Module 11 + Module 13 | feature/ISSUE-0043-modem-frame-symbol-protocol | v0.2.0 target | M2 real modem prototype follow-up | ISSUE-0041 | 2026-06-29 |
| ISSUE-0044 | closed | P0 | module:12,15,13 | prototype modem metrics separation | Module 12 + Module 15 + Module 13 | feature/ISSUE-0044-prototype-modem-metrics-separation | v0.2.0 target | M2 real modem prototype follow-up | ISSUE-0041, ISSUE-0043 | 2026-06-29 |
| ISSUE-0045 | closed | P1 | qa,project,module:06,07,09 | M3 planning for OFDM/FEC/sync | Manager + QA + Module 06 + Module 07 + Module 09 | docs/ISSUE-0045-m3-planning-ofdm-fec-sync | v0.3.0 target | M3 planning | ISSUE-0041, ISSUE-0042, ISSUE-0043, ISSUE-0044 | 2026-06-29 |
| ISSUE-0046 | complete | P0 | module:07,02,11,13 | OFDM carrier layout and baseband frame prototype | Module 07 + Module 02 + Module 11 + Module 13 | feature/ISSUE-0046-ofdm-carrier-layout-baseband-frame | v0.3.0 target | M3-A low-risk groundwork | ISSUE-0045, ISSUE-0043, ISSUE-0042 | 2026-06-29 |
| ISSUE-0047 | open | P0 | module:12,15,07,13 | occupied bandwidth validation harness | Module 12 + Module 15 + Module 07 + Module 13 | feature/ISSUE-0047-occupied-bandwidth-validation | v0.3.0 target | M3-A low-risk groundwork | ISSUE-0045, ISSUE-0046 | 2026-06-29 |
| ISSUE-0048 | open | P1 | module:09,07,02 | QPSK timing/sync placeholder | Module 09 + Module 07 + Module 02 | feature/ISSUE-0048-qpsk-timing-sync-placeholder | v0.3.0 target | M3-B sync placeholder | ISSUE-0045, ISSUE-0046 | 2026-06-29 |
| ISSUE-0049 | closed | P0 | module:06,02,12,15 | FEC placeholder and UEP policy RFC | Module 06 + Module 02 + Module 12 + Module 15 | docs/ISSUE-0049-fec-placeholder-uep-policy-rfc | v0.3.0 target | M3-A low-risk groundwork | ISSUE-0045, ISSUE-0044 | 2026-06-29 |
| ISSUE-0050 | open | P1 | module:06,07,11 | frame interleaver placeholder | Module 06 + Module 07 + Module 11 | feature/ISSUE-0050-frame-interleaver-placeholder | v0.3.0 target | M3-B interleaver placeholder | ISSUE-0045, ISSUE-0049 | 2026-06-29 |
| ISSUE-0051 | open | P0 | module:08,13,11,15 | channel impairment regression expansion | Module 08 + Module 13 + Module 11 + Module 15 | feature/ISSUE-0051-channel-impairment-regression | v0.3.0 target | M3-A low-risk groundwork | ISSUE-0045, ISSUE-0047 | 2026-06-29 |
| ISSUE-0052 | open | P1 | module:13,15,12,16 | M3 smoke campaign and report | Module 13 + Module 15 + Module 12 + QA | feature/ISSUE-0052-m3-smoke-campaign-report | v0.3.0 target | M3-B smoke/report | ISSUE-0046, ISSUE-0047, ISSUE-0049, ISSUE-0051 | 2026-06-29 |
| ISSUE-0053 | open | P0 | qa,project | M3 audit | Manager + QA | docs/ISSUE-0053-m3-audit | v0.3.0 target | M3 audit | ISSUE-0052 | 2026-06-29 |

## M1-A integration status

- ISSUE-0003 is integrated into local `develop` through `eba941a`.
- ISSUE-0004 M0 audit passed; reports exist under root `reports/` and Module 16 QA `reports/`.
- ISSUE-0005, ISSUE-0006, ISSUE-0007, and ISSUE-0008 are integrated locally into `develop`.
- ISSUE-0009, ISSUE-0010, ISSUE-0012, and ISSUE-0013 are startable from local `develop`.
- ISSUE-0011 remains held until ISSUE-0010 is complete.
- Remote `origin` is configured, but prior push was blocked by GitHub permission. Remote `develop` is not published.

## M1-B integration status

- ISSUE-0014, ISSUE-0010, ISSUE-0013, and ISSUE-0009 are integrated locally into `develop`.
- The M1-B instruction text used ISSUE-0013 for metrics and ISSUE-0012 for emulator, but existing issue files are source of truth: metrics is ISSUE-0014 and emulator is ISSUE-0013.
- ISSUE-0011, ISSUE-0012, and ISSUE-0015 are startable from local `develop`.
- ISSUE-0012 workers must initialize `external/codec2` before official adapter work.
- ISSUE-0016 remains held until remaining M1 work completes.

## M1-C integration status

- ISSUE-0011, ISSUE-0012, and ISSUE-0015 are integrated locally into `develop`.
- Merge commits: ISSUE-0011 `0f09a49`, ISSUE-0012 `5b48e62`, ISSUE-0015 `1710785`.
- Final M1-C local CI passed with 9 CTest tests.
- Push/PR not performed from this workspace; prior publication is blocked by GitHub permission.
- ISSUE-0016 M1 audit is now startable from local `develop`.

## M1 audit decision

- ISSUE-0016 completed with decision `GO_WITH_LIMITATIONS`.
- Root project version and M1-touched module versions are recorded as `v0.1.0` where applicable.
- M2 planning may begin, but official FreeDV waveform execution, SSB audio export, sweep CLI/TOML parsing, and remote publication remain follow-up items.

## M2 planning status

- ISSUE-0017 through ISSUE-0023 define M2 700F candidate exploration.
- M2-A startable planning/design work: ISSUE-0018 after ISSUE-0017 is merged.
- M2 implementation issues ISSUE-0019, ISSUE-0020, ISSUE-0021, and ISSUE-0022 must not begin candidate code until ISSUE-0018 defines boundaries and test expectations.
- ISSUE-0023 remains held until M2 candidate work completes.

## M2-A integration status

- ISSUE-0018 is integrated locally into `develop` as merge commit `59629d4`.
- Local CI after ISSUE-0018 passed with 10 CTest tests.
- A later manager request asked to use ISSUE-0019 through ISSUE-0022 for scoring/channel/sweep/report, but those issue numbers already exist as FEC/OFDM/sync/AI triage. Existing issue files remain source of truth.
- Requested scoring/channel/sweep/report work is split into ISSUE-0024 through ISSUE-0027.
- ISSUE-0023 remains held until the current M2 implementation issue set and follow-up comparison/report issue set are resolved or explicitly reprioritized.

## M2 issue map

- `docs/coordination/m2_issue_map.md` records the canonical M2 issue-number mapping.
- Future requests that conflict with existing issue meaning should create new follow-up issues rather than redefining existing issue files.

## M2-G waveform prototype planning status

- ISSUE-0038 defines the guarded RFC for making only `freedv700f_a_balanced`
  waveform-capable as an experimental prototype.
- ISSUE-0039 implements that first minimal 700F-A toy waveform prototype while
  preserving 700F-B/C surrogate semantics.
- ISSUE-0040 audits the post-700F-A state and records
  `M2_GO_TO_REAL_MODEM_RFC`.
- ISSUE-0041 defines the real 700F modem RFC and selects Option B followed by
  Option C: minimal QPSK-like baseband prototype first, then OFDM/FEC/sync
  expansion after boundaries stabilize.
- ISSUE-0042 through ISSUE-0045 are the follow-up dispatch set. Start ISSUE-0043
  first when possible; ISSUE-0044 may run in parallel if decoupled from source
  protocol types. ISSUE-0042 should wait for the minimal protocol boundary.
- ISSUE-0043, ISSUE-0044, and ISSUE-0042 are integrated into `develop`.
  M2-H smoke/report passed in both default Codec2 OFF and optional Codec2 ON
  paths. 700F-A is a limited `real_modem_prototype`; 700F-B/C remain surrogate.
- Final Manager handoff for this audit/RFC pass is
  `docs/handoff/ISSUE-0040-0041-real-700f-modem-rfc.md`.
- Final Manager handoff for the ISSUE-0042 through ISSUE-0044 integration pass
  is `docs/handoff/ISSUE-0042-0045-qpsk-prototype-and-m3-plan.md`.
- 700F-B and 700F-C remain ISSUE-0032 surrogate rows until later issues upgrade
  them explicitly.
- Real 700F candidate downselect remains prohibited after ISSUE-0040 until a
  later audit authorizes performance-valid real modem rows.

## M2-B integration status

- ISSUE-0024, ISSUE-0025, ISSUE-0026, and ISSUE-0027 are integrated locally into `develop`.
- Merge commits: ISSUE-0024 `bcad913`, ISSUE-0025 `8a2714e`, ISSUE-0026 `4ce1e3f`, ISSUE-0027 `088b7e0`.
- Final M2-B local CI passed with 12 CTest tests.
- M2-B smoke produced JSON/CSV artifacts, but all 30 campaign rows were skipped because campaign modes are not registered in the smoke runner.
- ISSUE-0028 tracks the mode registry bridge needed before M2 audit can make a useful candidate downselect.
- ISSUE-0023 may start only as a limited audit; candidate downselect should wait for ISSUE-0028 or an explicit decision to audit the skipped-only M2-B state.

## M2-C integration status

- ISSUE-0028 bridges M2 campaign modes to SSB, FreeDV emulator, and 700F
  candidate profile factories.
- M2-C smoke is no longer all skipped: default smoke produces completed SSB,
  descriptor-only emulator, and profile-only 700F rows, while official FreeDV
  rows remain explicit skips.
- Completed descriptor/profile-only rows do not claim waveform performance or a
  real candidate downselect.
- ISSUE-0023 can start as a limited audit after ISSUE-0028 integration; full
  downselect remains blocked by waveform-capable 700F candidates and official
  baseline roundtrip work.

## M2 limited audit decision

- ISSUE-0023 completed with decision `M2_GO_WITH_LIMITATIONS`.
- M2-D follow-up work may start.
- Real candidate downselect remains prohibited because current 700F rows are
  profile-only, 700D/700E emulator rows are descriptor-only, official FreeDV
  waveform roundtrip is incomplete, SSB audio export is incomplete, and report
  generation lacks a JSON loader/CLI.

## M2-D planning status

- ISSUE-0029, ISSUE-0030, and ISSUE-0031 are the low-risk first wave.
- ISSUE-0032 through ISSUE-0035 are planned follow-ups and should not be merged
  ahead of the first wave unless Manager explicitly reprioritizes.
- M2-D dispatch plan: `docs/coordination/m2d_dispatch_plan.md`.

## M2-D partial integration status

- ISSUE-0029, ISSUE-0030, and ISSUE-0031 are integrated locally into `develop`.
- Final integrated local CI passed with 20 CTest tests.
- CLI smoke ran through `f700f-sweep`; comparison Markdown was generated through
  `f700f-report`.
- Real candidate downselect remains prohibited.

## M3 planning status

- ISSUE-0045 completed M3 OFDM/FEC/sync planning and created ISSUE-0046 through
  ISSUE-0053.
- M3 target is `v0.3.0` on compatibility line `3`.
- M3-A first-wave startable issues are ISSUE-0049, ISSUE-0046, ISSUE-0047, and
  ISSUE-0051.
- M3-B/audit issues ISSUE-0048, ISSUE-0050, ISSUE-0052, and ISSUE-0053 remain
  held until M3-A boundaries land.
- Real 700F downselect remains prohibited.


# M2 Issue Map

## Source Of Truth

Existing issue files are the source of truth for issue number meaning, branch name, owner module, dependencies, and acceptance criteria.

If a later manager instruction reuses an existing issue number for a different meaning, do not rewrite the existing issue. Create a new follow-up issue instead and record the mapping here.

## Current Map

| Issue | Meaning | Status | Notes |
|---|---|---|---|
| ISSUE-0018 | 700F candidate architecture and profile descriptors | complete | Integrated locally into `develop` as `59629d4`. |
| ISSUE-0019 | FEC UEP engine prototype | open | Existing issue file is source of truth; do not redefine as scoring work. |
| ISSUE-0020 | OFDM waveform candidate prototype | open | Existing issue file is source of truth; do not redefine as channel matrix work. |
| ISSUE-0021 | sync and impairment estimator prototype | open | Existing issue file is source of truth; do not redefine as sweep campaign work. |
| ISSUE-0022 | AI-assisted sweep triage | open | Existing issue file is source of truth; do not redefine as baseline report work. |
| ISSUE-0023 | M2 audit and candidate downselect | limited-start | Limited audit can start after ISSUE-0028 smoke bridge; real candidate downselect remains blocked until waveform-capable 700F and official baselines exist. |
| ISSUE-0024 | M2 scoring model and comparison policy | complete | Follow-up issue for requested scoring path. |
| ISSUE-0025 | M2 expanded channel condition matrix | complete | Follow-up issue for requested channel matrix path. |
| ISSUE-0026 | M2 sweep campaign runner config | complete | Follow-up issue for requested sweep campaign path. |
| ISSUE-0027 | M2 baseline comparison report generation | complete | Follow-up issue for requested comparison report path. |
| ISSUE-0028 | M2-C campaign mode registry bridge | complete | M2 smoke modes register SSB/emulator/profile factories, produce completed metadata-only/profile-only rows, and keep official FreeDV explicit skips. |
| ISSUE-0029 | M2-D SSB audio export | complete | First-wave low-risk tooling/readiness issue integrated locally. |
| ISSUE-0030 | M2-D sweep CLI and config loader | complete | First-wave low-risk tooling issue integrated locally. |
| ISSUE-0031 | M2-D report JSON loader and CLI | complete | First-wave low-risk reporting issue integrated locally. |
| ISSUE-0032 | M2-D 700F candidate minimal behavior bridge | complete | 700F A/B/C rows complete as surrogate/not-real-modem/not-downselect-valid evidence. |
| ISSUE-0033 | M2-D 700D/700E emulator minimal behavior bridge | complete | 700D/700E emulator rows complete as non-official emulated-surrogate evidence and remain downselect-invalid. |
| ISSUE-0034 | M2-D official FreeDV waveform roundtrip | complete | Source-level Codec2/FreeDV API findings and upstream CLI 700D/700E roundtrip probe recorded; direct F700F runtime linkage remains follow-up work. |
| ISSUE-0035 | Clock-skew warning investigation | complete | Documented as non-blocking when local CI, CTest, version, and governance checks pass. |
| ISSUE-0036 | Codec2 runtime linkage and Mode-boundary official FreeDV roundtrip | complete | Optional Codec2-on builds import/link the pinned runtime and complete 700D/700E Mode-boundary roundtrip smoke rows; default Codec2-off CI remains green. |
| ISSUE-0037 | Codec2-on CI and official regression smoke | complete | Added a local Codec2-on wrapper plus Linux Actions job that initializes the submodule, runs ON CTest, validates official 700D/700E smoke completion, and keeps default OFF CI submodule-independent. |
| ISSUE-0038 | 700F waveform-capable prototype RFC | complete | Defines the guarded metadata and reporting boundary for upgrading only `freedv700f_a_balanced` to a first waveform-capable prototype while keeping B/C surrogate and downselect prohibited. |
| ISSUE-0039 | 700F-A minimal waveform-capable prototype | complete | Implements a toy waveform-capable 700F-A path with prototype metadata while keeping 700F-B/C surrogate and real downselect prohibited. |
| ISSUE-0040 | M2 audit update after 700F-A prototype | complete | Audits the post-ISSUE-0039 state, confirms Codec2 OFF/ON local CI, keeps real downselect prohibited, and authorizes a real modem architecture RFC. |
| ISSUE-0041 | real 700F modem architecture RFC | complete | Defines the real modem prototype boundary and selects Option B then Option C; implementation and downselect remain follow-up-gated. |
| ISSUE-0042 | 700F-A minimal QPSK baseband prototype | open | First Option B implementation issue; should wait for the ISSUE-0043 protocol boundary where practical. |
| ISSUE-0043 | modem frame and symbol protocol | open | Source-level protocol boundary for modem input/output frames, symbol blocks, baseband frames, and prototype status. |
| ISSUE-0044 | prototype modem metrics separation | open | Keeps limited prototype modem diagnostics separate from final performance and downselect scoring. |
| ISSUE-0045 | M3 planning for OFDM/FEC/sync | open | Planning-only bridge toward Option C; no M3 implementation until M2 real prototype boundaries are audited. |

## Collision Rule

When issue number meaning conflicts:

1. Keep the existing issue file unchanged.
2. Create a new issue for the newly requested work.
3. Update `issues/README.md`, milestone docs, dispatch docs, and handoff docs.
4. Record the reason in this map.

## M2-B Dispatch Direction

For the scoring/channel/report path, ISSUE-0024 through ISSUE-0028 are integrated.
ISSUE-0023 can start as a limited M2 audit of the current bridge outputs. It must
not perform a real candidate downselect until waveform-capable 700F rows and
official baseline roundtrip rows are available.

## M2-D Direction

ISSUE-0023 decided `M2_GO_WITH_LIMITATIONS`. Start ISSUE-0029 through ISSUE-0031
first. Keep ISSUE-0032 through ISSUE-0035 planned until the first-wave tooling
path is available or Manager explicitly reprioritizes.

ISSUE-0029 through ISSUE-0037 are integrated locally. ISSUE-0034 is complete as
a guarded research checkpoint, and ISSUE-0036 adds direct optional F700F Codec2
runtime linkage for official 700D/700E Mode-boundary smoke rows. ISSUE-0037 makes
that official evidence repeatable in local/CI regression. Real candidate
downselect remains prohibited until waveform-capable 700F rows exist.

ISSUE-0038 is the RFC gate for that next step. It allows only 700F-A to become a
first waveform-capable prototype in a follow-up issue and keeps prototype rows
performance-invalid and downselect-invalid.

ISSUE-0039 is that first implementation. It must not upgrade 700F-B/C and must
not turn prototype rows into real performance or downselect evidence.

ISSUE-0040 records decision `M2_GO_TO_REAL_MODEM_RFC`. The next work should be
architecture/RFC only; real modem implementation and candidate downselect remain
blocked until explicit follow-up gates.

ISSUE-0041 completes that RFC and authorizes follow-up issue expansion for a
minimal QPSK-like baseband prototype, modem frame/symbol protocol, prototype
metrics separation, and M3 OFDM/FEC/sync planning.

Dispatch is recorded in `docs/coordination/real_700f_modem_dispatch_plan.md`.
Real 700F downselect remains prohibited across ISSUE-0042 through ISSUE-0045.

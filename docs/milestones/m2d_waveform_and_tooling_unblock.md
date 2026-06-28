# M2-D Waveform and Tooling Unblock

## Purpose

M2-D follows the ISSUE-0023 `M2_GO_WITH_LIMITATIONS` decision. It moves the
project from profile/descriptor-only smoke readiness toward tool-driven sweeps
and clearly labeled minimal behavior. It does not authorize real candidate
downselect.

## Entry Criteria

- ISSUE-0023 limited M2 audit complete.
- ISSUE-0028 M2-C smoke bridge complete.
- Local CI passing with default `F700F_ENABLE_CODEC2=OFF`.

## Issue Plan

| Issue | Title | Status | Primary owner | Branch |
|---|---|---|---|---|
| ISSUE-0029 | SSB audio export | complete-local | Module 10 + Module 11 + Module 15 | `feature/ISSUE-0029-ssb-audio-export` |
| ISSUE-0030 | sweep CLI and config loader | complete-local | Module 13 + Manager | `feature/ISSUE-0030-sweep-cli-config-loader` |
| ISSUE-0031 | report JSON loader and CLI | complete-local | Module 15 + Module 12 | `feature/ISSUE-0031-report-json-loader-cli` |
| ISSUE-0032 | 700F candidate minimal behavior bridge | complete-local | Module 02 + candidate modules | `feature/ISSUE-0032-700f-candidate-minimal-behavior` |
| ISSUE-0033 | 700D/700E emulator minimal behavior bridge | complete-local | Module 10 + Module 11 + Module 13 | `feature/ISSUE-0033-700d-700e-emulator-minimal-behavior` |
| ISSUE-0034 | official FreeDV waveform roundtrip | complete-local | Module 05 + Module 10 | `research/ISSUE-0034-official-freedv-waveform-roundtrip` |
| ISSUE-0035 | clock-skew warning investigation | complete-local | Module 01 + Manager | `chore/ISSUE-0035-clock-skew-warning-investigation` |
| ISSUE-0036 | Codec2 runtime linkage and Mode-boundary official FreeDV roundtrip | complete-local | Module 05 + Module 10 + Module 11 + Module 13 | `feature/ISSUE-0036-codec2-runtime-linkage-mode-roundtrip` |
| ISSUE-0037 | Codec2-on CI and official regression smoke | planned | Module 01 + Module 05 + Module 10 + Module 13 | `chore/ISSUE-0037-codec2-on-ci-official-regression-smoke` |

## Downselect Rule

Real candidate downselect remains prohibited until waveform-capable 700F work
produces valid performance rows and official runtime evidence is in a recurring
regression path. Current M2-E/M2-F rows distinguish real, surrogate, emulator,
official-runtime, and official-unavailable states, but do not provide a real
downselect basis.

## Partial Integration

ISSUE-0029 through ISSUE-0031 are integrated locally into `develop`. CLI smoke can
run through `f700f-sweep`, and comparison Markdown can be generated through
`f700f-report`. The default M2-D smoke still reports no real downselect.

## M2-E Integration

ISSUE-0032 through ISSUE-0035 are integrated locally into `develop`. The M2-E
smoke sweep produced 27 rows: 21 completed, 6 skipped, 0 failed. 700F candidates
complete as synthetic surrogate rows, 700D/700E emulators complete as
non-official emulated-surrogate rows, and official FreeDV rows remain skipped in
the default Codec2-off smoke.

## M2-F Codec2 Runtime Follow-up

ISSUE-0036 is integrated locally. Default Codec2-off CI remains green. Optional
Codec2-on builds import/link the pinned `external/codec2` runtime and complete
official 700D/700E Mode-boundary smoke rows. ISSUE-0037 tracks making this
non-default Codec2-on path recurring.

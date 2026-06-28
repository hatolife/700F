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
| ISSUE-0029 | SSB audio export | startable | Module 10 + Module 11 + Module 15 | `feature/ISSUE-0029-ssb-audio-export` |
| ISSUE-0030 | sweep CLI and config loader | startable | Module 13 + Manager | `feature/ISSUE-0030-sweep-cli-config-loader` |
| ISSUE-0031 | report JSON loader and CLI | startable | Module 15 + Module 12 | `feature/ISSUE-0031-report-json-loader-cli` |
| ISSUE-0032 | 700F candidate minimal behavior bridge | planned | Module 02 + candidate modules | `feature/ISSUE-0032-700f-candidate-minimal-behavior` |
| ISSUE-0033 | 700D/700E emulator minimal behavior bridge | planned | Module 10 + Module 11 + Module 13 | `feature/ISSUE-0033-700d-700e-emulator-minimal-behavior` |
| ISSUE-0034 | official FreeDV waveform roundtrip | planned | Module 05 + Module 10 | `research/ISSUE-0034-official-freedv-waveform-roundtrip` |
| ISSUE-0035 | clock-skew warning investigation | planned | Module 01 + Manager | `chore/ISSUE-0035-clock-skew-warning-investigation` |

## Downselect Rule

Real candidate downselect remains prohibited until ISSUE-0032/0033 or official
baseline work produces waveform-capable rows with reporting that distinguishes
real, surrogate, emulator, and official results.

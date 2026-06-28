# Branch ownership

| Issue | Branch | Owner | Worktree | Status | Merge target |
|---|---|---|---|---|---|
| ISSUE-0003 | docs/ISSUE-0003-m1-planning | Manager | ../f700f-wt-issue-0003 | merged-local | develop |
| ISSUE-0004 | docs/ISSUE-0004-m0-audit | Manager / QA | ../f700f-wt-issue-0004 | final-checkpoint | develop |
| ISSUE-0005 | docs/ISSUE-0005-github-actions-ci | Module 01 / Manager | TBD | ready_m1a | develop |
| ISSUE-0006 | docs/ISSUE-0006-governance-structure | Manager + all modules | TBD | ready_m1a | develop |
| ISSUE-0007 | research/ISSUE-0007-codec2-submodule-pin | Manager + Module 05 + Module 10 | TBD | ready_m1a | develop |
| ISSUE-0008 | feature/ISSUE-0008-mode-interface-v0-1 | Module 02 + Module 03 + Manager | TBD | ready_m1a | develop |
| ISSUE-0017 | docs/ISSUE-0017-m2-planning | Manager | ../f700f-wt-issue-0017 | active-local | develop |
| ISSUE-0018 | docs/ISSUE-0018-700f-candidate-architecture-rfc | Module 02 + Manager | ../f700f-wt-issue-0018 | ready_after_0017 | develop |
| ISSUE-0019 | feature/ISSUE-0019-fec-uep-prototype | Module 06 + Module 02 | ../f700f-wt-issue-0019 | held_for_0018 | develop |
| ISSUE-0020 | feature/ISSUE-0020-ofdm-waveform-candidate | Module 07 + Module 09 | ../f700f-wt-issue-0020 | held_for_0018 | develop |
| ISSUE-0021 | feature/ISSUE-0021-sync-impairment-estimator | Module 09 + Module 07 | ../f700f-wt-issue-0021 | held_for_0018_0020 | develop |
| ISSUE-0022 | research/ISSUE-0022-ai-sweep-triage | Module 14 + Module 13 | ../f700f-wt-issue-0022 | held_for_0018 | develop |
| ISSUE-0023 | docs/ISSUE-0023-m2-audit-downselect | Manager + QA | ../f700f-wt-issue-0023 | held_for_m2_completion | develop |
| ISSUE-0029 | feature/ISSUE-0029-ssb-audio-export | Module 10 + Module 11 + Module 15 | ../700F-ISSUE-0029 | ready_m2d_first_wave | develop |
| ISSUE-0030 | feature/ISSUE-0030-sweep-cli-config-loader | Module 13 + Manager | ../700F-ISSUE-0030 | ready_m2d_first_wave | develop |
| ISSUE-0031 | feature/ISSUE-0031-report-json-loader-cli | Module 15 + Module 12 | ../700F-ISSUE-0031 | ready_m2d_first_wave | develop |
| ISSUE-0032 | feature/ISSUE-0032-700f-candidate-minimal-behavior | Module 02 + candidate modules | ../700F-ISSUE-0032 | merged-local | develop |
| ISSUE-0033 | feature/ISSUE-0033-700d-700e-emulator-minimal-behavior | Module 10 + Module 11 + Module 13 | ../700F-ISSUE-0033 | merged-local | develop |
| ISSUE-0034 | research/ISSUE-0034-official-freedv-waveform-roundtrip | Module 05 + Module 10 | ../700F-ISSUE-0034 | merged-local | develop |
| ISSUE-0035 | chore/ISSUE-0035-clock-skew-warning-investigation | Module 01 + Manager | ../700F-ISSUE-0035 | merged-local | develop |
| ISSUE-0036 | feature/ISSUE-0036-codec2-runtime-linkage-mode-roundtrip | Module 05 + Module 10 + Module 11 + Module 13 | ../700F-ISSUE-0036 | merged-local | develop |
| ISSUE-0037 | chore/ISSUE-0037-codec2-on-ci-official-regression-smoke | Module 01 + Module 05 + Module 10 + Module 13 | ../700F-ISSUE-0037 | merged-local | develop |

## M1-A dispatch worktrees

| Issue | Worktree | Recommended model | Notes |
|---|---|---|---|
| ISSUE-0005 | ../f700f-wt-issue-0005 | GPT-5.3-Codex-Spark; gpt-5.5-low for CI failures; gpt-5.5-medium for Windows CI | Start from `develop`; CI should land early. |
| ISSUE-0006 | ../f700f-wt-issue-0006 | GPT-5.3-Codex-Spark | Start from `develop`; governance-only unless issue expands scope. |
| ISSUE-0007 | ../f700f-wt-issue-0007 | gpt-5.5-medium; gpt-5.5-xhigh for Codec2/FreeDV internals | Start from `develop`; no 700D/700E implementation yet. |
| ISSUE-0008 | ../f700f-wt-issue-0008 | gpt-5.5-medium; gpt-5.5-xhigh for interface boundary decisions | Start from `develop`; blocks ISSUE-0009 and most later work. |

Remote `develop` is not published. `origin` is configured, but previous push was blocked by GitHub permission.

## M2-A dispatch worktrees

| Issue | Worktree | Recommended model | Notes |
|---|---|---|---|
| ISSUE-0018 | ../f700f-wt-issue-0018 | gpt-5.5-xhigh for interface boundaries; gpt-5.5-medium for docs/tests | Start after ISSUE-0017 merges to local `develop`. |
| ISSUE-0019 | ../f700f-wt-issue-0019 | gpt-5.5-medium; gpt-5.5-xhigh for coding theory choices | Hold until ISSUE-0018 lands. |
| ISSUE-0020 | ../f700f-wt-issue-0020 | gpt-5.5-medium; gpt-5.5-xhigh for waveform design | Hold until ISSUE-0018 lands. |
| ISSUE-0021 | ../f700f-wt-issue-0021 | gpt-5.5-medium | Hold until ISSUE-0018 and a minimal ISSUE-0020 waveform contract land. |
| ISSUE-0022 | ../f700f-wt-issue-0022 | GPT-5.3-Codex-Spark for notes; gpt-5.5-medium for integration | Hold implementation until ISSUE-0018 lands. |
| ISSUE-0023 | ../f700f-wt-issue-0023 | gpt-5.5-medium; gpt-5.5-xhigh for downselect | Hold until M2 work completes. |

## M2-D dispatch worktrees

| Issue | Worktree | Recommended model | Notes |
|---|---|---|---|
| ISSUE-0029 | ../700F-ISSUE-0029 | GPT-5.3-Codex-Spark; fallback gpt-5.5-low | Start first wave from `develop`; SSB audio artifact export. |
| ISSUE-0030 | ../700F-ISSUE-0030 | gpt-5.5-medium; fallback gpt-5.5-xhigh | Start first wave from `develop`; sweep CLI/config loading. |
| ISSUE-0031 | ../700F-ISSUE-0031 | gpt-5.5-medium; fallback gpt-5.5-xhigh | Start first wave from `develop`; report JSON loader/CLI. |
| ISSUE-0032 | ../700F-ISSUE-0032 | gpt-5.5-xhigh | Merged locally in M2-E. |
| ISSUE-0033 | ../700F-ISSUE-0033 | gpt-5.5-xhigh | Merged locally in M2-E. |
| ISSUE-0034 | ../700F-ISSUE-0034 | gpt-5.5-xhigh | Merged locally in M2-E; direct runtime linkage remains ISSUE-0036. |
| ISSUE-0035 | ../700F-ISSUE-0035 | GPT-5.3-Codex-Spark | Merged locally in M2-E. |
| ISSUE-0036 | ../700F-ISSUE-0036 | gpt-5.5-xhigh | Merged locally; optional Codec2 runtime path keeps default OFF green and passes ON smoke locally. |
| ISSUE-0037 | ../700F-ISSUE-0037 | gpt-5.5-medium | Merged locally; Codec2-on official smoke is recurring through local wrapper and Linux Actions job. |

# M2-D Dispatch Plan

## First Wave

| Issue | Recommended model | Branch | Worktree | Owner module | Prior dependency |
|---|---|---|---|---|---|
| ISSUE-0029 | GPT-5.3-Codex-Spark; fallback gpt-5.5-low | `feature/ISSUE-0029-ssb-audio-export` | `../700F-ISSUE-0029` | 10, 11, 15 | ISSUE-0023, ISSUE-0028 |
| ISSUE-0030 | gpt-5.5-medium; fallback gpt-5.5-xhigh | `feature/ISSUE-0030-sweep-cli-config-loader` | `../700F-ISSUE-0030` | 13, project | ISSUE-0023, ISSUE-0028 |
| ISSUE-0031 | gpt-5.5-medium; fallback gpt-5.5-xhigh | `feature/ISSUE-0031-report-json-loader-cli` | `../700F-ISSUE-0031` | 15, 12 | ISSUE-0023, ISSUE-0027, ISSUE-0028 |

## Later Wave

| Issue | Recommended model | Branch | Worktree | Owner module | Start condition |
|---|---|---|---|---|---|
| ISSUE-0032 | gpt-5.5-xhigh | `feature/ISSUE-0032-700f-candidate-minimal-behavior` | `../700F-ISSUE-0032` | 02, 06, 07, 09, 11, 13 | Start after tooling first wave or explicit Manager approval. |
| ISSUE-0033 | gpt-5.5-xhigh | `feature/ISSUE-0033-700d-700e-emulator-minimal-behavior` | `../700F-ISSUE-0033` | 10, 11, 13 | Start after reporting path can label emulator/surrogate rows. |
| ISSUE-0034 | gpt-5.5-xhigh | `research/ISSUE-0034-official-freedv-waveform-roundtrip` | `../700F-ISSUE-0034` | 05, 10 | Start as research/integration with Codec2 guards. |
| ISSUE-0035 | GPT-5.3-Codex-Spark | `chore/ISSUE-0035-clock-skew-warning-investigation` | `../700F-ISSUE-0035` | 01 | Low priority; warnings are non-blocking. |

## Shared Instructions

All workers must read root `AGENTS.md`, this dispatch plan, the target issue,
module `AGENTS.md`, and relevant specs under `docs/specs/`. Default CI must
remain green with `F700F_ENABLE_CODEC2=OFF`. Profile-only and descriptor-only
rows must never be reported as real RF/audio performance.

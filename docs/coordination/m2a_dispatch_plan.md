# M2-A Dispatch Plan

## Status

- Source branch for dispatch: local `develop` after ISSUE-0017 merge
- Prepared by: ISSUE-0017 M2 planning
- M2-A state: planning/design only until ISSUE-0018 lands
- Remote state: `origin` is configured, but previous push was blocked by GitHub permission
- PR requirement: none unless a human explicitly asks

## Dispatch Table

| Issue | Recommended model | Branch | Worktree | Owner module | Prerequisite |
|---|---|---|---|---|---|
| ISSUE-0018 | gpt-5.5-xhigh for interface boundaries; gpt-5.5-medium for doc/test drafting | `docs/ISSUE-0018-700f-candidate-architecture-rfc` | `../f700f-wt-issue-0018` | Module 02 + Manager | ISSUE-0017 |
| ISSUE-0019 | gpt-5.5-medium; escalate to gpt-5.5-xhigh for coding theory choices | `feature/ISSUE-0019-fec-uep-prototype` | `../f700f-wt-issue-0019` | Module 06 + Module 02 | ISSUE-0018 |
| ISSUE-0020 | gpt-5.5-medium; escalate to gpt-5.5-xhigh for modem waveform design | `feature/ISSUE-0020-ofdm-waveform-candidate` | `../f700f-wt-issue-0020` | Module 07 + Module 09 | ISSUE-0018 |
| ISSUE-0021 | gpt-5.5-medium | `feature/ISSUE-0021-sync-impairment-estimator` | `../f700f-wt-issue-0021` | Module 09 + Module 07 | ISSUE-0018, ISSUE-0020 |
| ISSUE-0022 | GPT-5.3-Codex-Spark for tooling notes; gpt-5.5-medium for metric/sweep integration | `research/ISSUE-0022-ai-sweep-triage` | `../f700f-wt-issue-0022` | Module 14 + Module 13 | ISSUE-0015, ISSUE-0018 |
| ISSUE-0023 | gpt-5.5-medium; escalate to gpt-5.5-xhigh for downselect decision | `docs/ISSUE-0023-m2-audit-downselect` | `../f700f-wt-issue-0023` | Manager + QA | ISSUE-0018 through ISSUE-0022 |

## Dispatch Order

1. ISSUE-0018: define 700F candidate architecture and boundaries.
2. ISSUE-0019 and ISSUE-0020: begin after ISSUE-0018 lands.
3. ISSUE-0021: begin after ISSUE-0020 has a minimal waveform contract.
4. ISSUE-0022: may prepare research notes after ISSUE-0018, but should not drive candidate selection before candidate artifacts exist.
5. ISSUE-0023: hold until M2 implementation issues complete.

## Hold Line

- Do not implement final 700F modem claims in M2-A planning.
- Do not present parameterized 700D/700E emulators as official baselines.
- Do not claim official FreeDV waveform parity until Codec2 waveform round trip is implemented and tested.
- Do not rely on sweep TOML parsing or a standalone CLI until follow-up work lands.

## Worker Instructions

- Branch from local `develop` after ISSUE-0017 is merged.
- Update the root issue before implementation.
- Create module-local issue files before module code changes.
- Record red tests or a documented TDD exception before implementation.
- Run `./tools/run_ci_local.sh` before handoff when practical.
- Record push/PR as blocked if GitHub permission still prevents publication.

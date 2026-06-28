# M2-D Partial Integration Report

## Summary

- Branch: `develop`
- Report time: 2026-06-29 00:37:46 +09:00
- Integrated issues: ISSUE-0029, ISSUE-0030, ISSUE-0031
- Merge commits: ISSUE-0029 plus ISSUE-0031 `f867d04`; ISSUE-0030 `bd15e2a`
- Local CI: passed with 20 CTest tests
- Push/PR: Manager did not push or create a PR. Subagents reported pushes for
  ISSUE-0030 and ISSUE-0031; this is recorded as a deviation from manager
  no-push instruction.

## Integrated Scope

- ISSUE-0029: decoded audio export path propagation through simulation/sweep
  summaries.
- ISSUE-0030: `f700f-sweep` CLI and minimal TOML-shaped config loader.
- ISSUE-0031: `f700f-report` CLI and sweep JSON/CSV report loader.

## Smoke

- CLI sweep succeeded: 21 completed, 0 failed, 6 skipped.
- CLI report succeeded:
  `reports/20260629_003746_m2d_candidate_comparison.md`.
- Real downselect: not possible.

## Known Gaps

- CLI smoke with default config has SSB audio export disabled.
- A manual `export_audio = true` CLI smoke check still reported `N/A` audio paths,
  so SSB audio artifact creation through CLI smoke needs follow-up validation.
- 700F candidates remain profile-only.
- 700D/700E emulators remain descriptor-only.
- Official FreeDV waveform roundtrip remains incomplete.
- Clock-skew warnings remain non-blocking.

## Next

Start ISSUE-0032 and ISSUE-0033 only with strict surrogate/emulator labeling.
ISSUE-0034 can start as guarded Codec2/FreeDV research. ISSUE-0035 remains low
priority.

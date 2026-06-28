# Handoff: ISSUE-0032 through ISSUE-0035 M2-E Integration

- Current branch: `develop`
- Latest integrated commit before final record commit: `b0d74e5`
- Remote: `origin` exists, but push/PR was not performed by request
- Completed issues: ISSUE-0032, ISSUE-0033, ISSUE-0034, ISSUE-0035
- Created follow-up issue: ISSUE-0036
- Local CI: passed
- CTest: 20/20 passed
- Version check: passed
- Governance check: passed

## What Changed

- 700F A/B/C candidate rows now complete as synthetic surrogate rows with
  `not_real_modem=true`, `downselect_valid=false`, and `performance_valid=false`.
- 700D/700E emulator rows now complete as deterministic non-official
  emulated-surrogate rows.
- Official FreeDV 700D/700E runtime research is documented, but direct F700F runtime
  linkage remains blocked.
- Clock-skew warnings are documented as non-blocking when local checks pass.

## M2-E Smoke

- Total rows: 27
- Completed: 21
- Skipped: 6
- Failed: 0
- 700F surrogate rows: 9
- 700D/700E emulator rows: 6
- Official FreeDV rows: 6 skipped
- SSB audio export: `N/A` in default smoke because `export_audio=false`

## Blockers

- Real downselect remains blocked.
- Direct F700F Codec2 runtime linkage is not implemented.
- Waveform-capable 700F candidate implementations are not implemented.
- Official FreeDV rows remain skipped in default Codec2-off smoke.

## Next Startable Work

- ISSUE-0036: Codec2 runtime linkage and Mode-boundary official FreeDV roundtrip.
- Later 700F waveform candidate implementation issues may start only with clear
  interface boundaries and must not treat surrogate readiness as real performance.

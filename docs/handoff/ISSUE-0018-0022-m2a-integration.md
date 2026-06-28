# ISSUE-0018-0022 M2-A Integration Handoff

## Current State

- Current branch: `develop`
- Latest M2-A merge before handoff commit: `59629d4`
- Integrated issue: ISSUE-0018
- Local CI: pass, 10 CTest tests
- Remote: `origin` is configured, but publication is blocked by GitHub permission

## Completed

- ISSUE-0018 defined and integrated the profile-only 700F candidate architecture.
- 700F-A/B/C candidate profiles are available as selectable descriptors.
- Candidate profile docs and tests are present.

## Not Integrated In This Pass

- ISSUE-0019 through ISSUE-0022 were not spawned for the requested scoring/channel/sweep/report tasks because their existing issue files define different work.
- No M2-A sweep smoke was run.
- No M2-A comparison report was generated.

## Follow-Up Issues Created

- ISSUE-0024: M2 scoring model and comparison policy
- ISSUE-0025: M2 expanded channel condition matrix
- ISSUE-0026: M2 sweep campaign runner config
- ISSUE-0027: M2 baseline comparison report generation

## Known Blockers

- Remote publication remains blocked.
- Official FreeDV waveform round trip remains incomplete.
- SSB audio export remains incomplete.
- Sweep CLI/TOML parser remains incomplete.
- M2 scoring/sweep/report work is not yet implemented.

## Next Work

Start ISSUE-0024 and ISSUE-0025 in parallel if the priority is scoring/channel/sweep/report. Start ISSUE-0019 and ISSUE-0020 if the priority is the original FEC/OFDM candidate implementation plan.

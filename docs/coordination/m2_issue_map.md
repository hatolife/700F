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

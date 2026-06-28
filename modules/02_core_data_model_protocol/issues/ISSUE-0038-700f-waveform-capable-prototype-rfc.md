# ISSUE-0038: 700F waveform-capable prototype RFC

- Status: complete
- Scope: module:02,07,11,12,13,15
- Owner: Module 02 + Module 07 + Module 13 + Module 15
- Branch: docs/ISSUE-0038-700f-waveform-prototype-rfc
- Target version: v0.2.0 target
- Milestone: M2 follow-up
- Depends: ISSUE-0032, ISSUE-0036, ISSUE-0037
- Last updated: 2026-06-29

## Module Boundary

Module 02 owns the candidate descriptor and Mode-boundary semantics for the RFC.
The RFC allows only `freedv700f_a_balanced` to advance from surrogate to the
first waveform-capable prototype in a follow-up implementation issue.

The RFC does not authorize final modem claims, performance scoring, or candidate
downselect. It defines metadata and guardrails that downstream sweep, scoring,
and reporting modules must preserve.

## Required Outcome

- Document the prototype descriptor fields and result-artifact metadata.
- Keep 700F-B and 700F-C under ISSUE-0032 surrogate semantics.
- Keep official FreeDV 700D/700E evidence separate from experimental 700F
  prototype rows.
- Leave implementation to ISSUE-0039 after this RFC is integrated.

## Result

Completed. The RFC is recorded in
`docs/specs/700f_waveform_prototype_rfc.md` and gates the follow-up ISSUE-0039
implementation. The Module 02 descriptor boundary must expose 700F-A as a
waveform-capable prototype only after ISSUE-0039 lands, while preserving B/C
surrogate semantics.

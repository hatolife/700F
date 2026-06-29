# Api Contract

Bootstrap placeholder for FEC and UEP Engine.

ISSUE-0049 defines the current public vocabulary for future Module 06 work:

- no-FEC: `fec_family=none`, no FEC/UEP policy is present.
- placeholder-FEC: `fec_family=placeholder`, metadata may describe policy
  intent, but no error correction is performed.
- real-FEC: future explicit family ids only after a scoped implementation issue.

Future UEP metadata may describe policy ids, important/less-important bit
classes, latency assumptions, interleaver policy references, and report validity
as documented in `docs/specs/700f_fec_uep_policy_plan.md`.

This module has no real FEC API in ISSUE-0049.

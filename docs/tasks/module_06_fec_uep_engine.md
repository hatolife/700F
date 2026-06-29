# Module 06: FEC and UEP Engine task brief

Module 06 owns the FEC and unequal error protection vocabulary for 700F
prototype work.

## Current M3 Scope

ISSUE-0049 is an RFC/documentation boundary. It defines:

- no-FEC: `fec_family=none`
- placeholder-FEC: `fec_family=placeholder`, metadata only, no correction
- future real-FEC: explicit future family ids, prohibited until a scoped issue
- UEP metadata for policy ids, important/less-important bit classes, latency
  assumptions, interleaver references, and report validity

Module 06 must not implement real FEC, final coding-family selection, parity
generation, or interleaver behavior under ISSUE-0049. ISSUE-0050 owns the frame
interleaver placeholder.

## Handoff Expectations

Future implementation issues should start with tests for descriptor/report
retention before adding behavior. Public API changes require updates to
`docs/protocol.md`, `docs/module_contracts.md`, and the relevant spec files.

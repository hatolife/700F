# Module 06 Specification

Module 06 owns future FEC and unequal error protection behavior for 700F
prototype work.

## ISSUE-0049 Boundary

The current boundary is metadata-only:

- `fec_family=none`: no FEC or UEP policy is active.
- `fec_family=placeholder`: future FEC/UEP intent may be reported, but no
  encoder, decoder, parity stream, correction, or erasure recovery exists.
- Future real-FEC family ids are prohibited until a separate issue adds tested
  behavior.

UEP policy metadata may name important and less-important bit classes, frame or
payload regions, latency assumptions, and the future ISSUE-0050 interleaver
policy reference. These names are planning vocabulary, not final codec or FEC
downselect.

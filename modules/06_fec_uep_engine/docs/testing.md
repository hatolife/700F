# Testing

ISSUE-0049 is planning/RFC work, so no source tests are required while the diff
is documentation-only.

If a future issue adds source descriptors or report fields, it should start with
tests for:

- no-FEC versus placeholder-FEC distinction
- `fec_family=placeholder` retention through descriptors/artifacts
- UEP policy id and bit-class metadata retention
- placeholder-FEC reporting as metadata-only and downselect-invalid

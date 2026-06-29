# Module 06: FEC and UEP Engine

Bootstrap skeleton for `06_fec_uep_engine`.

Current M3 policy is documented by ISSUE-0049:

- `fec_family=none` means no FEC or UEP policy is active.
- `fec_family=placeholder` means metadata-only FEC/UEP intent with no parity,
  correction, erasure recovery, or interleaver behavior.
- Real FEC family ids require a future scoped implementation issue with tests.

Module 06 owns the FEC/UEP vocabulary and future engine work. ISSUE-0049 does
not implement real FEC or authorize candidate downselect.

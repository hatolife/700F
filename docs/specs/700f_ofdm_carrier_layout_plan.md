# 700F OFDM Carrier Layout Plan

## Status

- Issue: ISSUE-0045
- Follow-up: ISSUE-0046

## Purpose

Define the minimal OFDM carrier-layout boundary for a future 700F-A prototype
without implementing a final OFDM modem.

## Candidate Metadata

The ISSUE-0046 boundary should be able to express:

- carrier count
- carrier spacing in hertz
- occupied bandwidth target in hertz
- data carrier count
- pilot carrier count
- guard carrier count
- center-frequency metadata
- future FEC placeholder state
- future sync placeholder state

## Validation

The first implementation should reject:

- zero carriers
- non-positive carrier spacing
- non-positive occupied bandwidth target
- layouts where data, pilot, and guard counts do not sum to total carriers

## Target Bandwidth

The M3 target remains 1.9 kHz occupied bandwidth as a planning and measurement
target. ISSUE-0046 records the target; ISSUE-0047 validates estimates.

## Non-Final Status

The OFDM layout boundary does not make 700F-A final, interoperable, or
downselect-valid.

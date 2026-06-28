# Changelog

## v0.2.0 - 2026-06-28
- Add ISSUE-0044 prototype modem classification and diagnostic fields while keeping
  waveform and real modem prototypes out of real performance scoring.
- Add ISSUE-0032 surrogate guardrail fields and keep synthetic surrogate readiness
  separate from real performance score and BER/FER availability.
- Add ISSUE-0024 interim M2 scoring model with completed/skipped/failed aggregation,
  dropout/latency/bandwidth penalties, BER/FER availability accounting, official
  unavailable handling, future metric slots, and profile-only snapshot support.
- Extend mode descriptor snapshots with append-only official/emulator/status fields
  needed by scoring and reporting consumers.

## v0.1.0 - 2026-06-27
- Define metrics/result artifact JSON+CSV schema (ISSUE-0014) with stable required fields and optional extension points.
- Add tolerant serializer/parsers and minimal unit tests for N/A BER/FER, skipped handling, and optional metrics.

## v0.0.1 - 2026-06-27
- Bootstrap Metrics and Scoring Engine module skeleton.

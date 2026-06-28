# Module Spec

Module 12 provides metrics artifact serialization and the interim M2 scoring model.

## Scoring Model

The ISSUE-0024 scorer compares modes using completed run ratio, skipped/failed
penalties, dropout, latency, RF bandwidth, BER/FER availability, official-unavailable
counts, audio-only N/A handling, profile-only snapshots, and slots for future ASR WER,
STOI, ESTOI, and subjective notes.

The project-level policy is documented in `docs/specs/scoring_model.md`.

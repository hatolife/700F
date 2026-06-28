# ISSUE-0024: M2 scoring model and comparison policy

## Status
done

## Scope
module:12

## Priority
P0

## Branch
- Base branch: develop
- Work branch: feature/ISSUE-0024-scoring-model
- Merge target: develop

## Target version and milestone
- Project version: v0.2.0 target
- Module version: v0.2.0
- Milestone: M2 700F Candidate Exploration

## Task
Implement the Module 12 portion of the interim M2 scoring model defined by root
ISSUE-0024.

## Acceptance criteria
- [x] Public scoring API exists in `f700f/scoring_model.hpp`.
- [x] Scoring tracks completed run ratio, skipped/failed penalties, dropout, latency,
  bandwidth, BER/FER availability, official-unavailable handling, audio-only N/A
  handling, future ASR WER/STOI/ESTOI slots, subjective note slots, and profile-only
  snapshots.
- [x] Deterministic contract tests cover empty results, completed/failed/skipped
  comparisons, dropout, latency, bandwidth penalty, BER/FER N/A, and profile snapshot.

## TDD record
- Red test added first in `tests/test_scoring_model.cpp`; initial focused build failed
  because `f700f/scoring_model.hpp` did not exist.

## Result
Implemented in ISSUE-0024 branch.

# Testing

Module 13 coverage lives in `tests/test_sweep_runner.cpp`.

Current checks include:

- stable mode/channel/seed expansion;
- skipped unavailable modes and failed-run continuation;
- aggregate JSON/CSV artifact emission;
- M1 smoke helper shape;
- ISSUE-0025 M2 smoke/full channel matrix helper shape;
- duplicate channel condition id rejection;
- malformed SNR/frequency-offset rejection;
- empty seed-list rejection.

# Limitations

- ISSUE-0024 scoring is an interim deterministic comparison policy, not a final M2
  downselect.
- ASR WER, STOI, ESTOI, and subjective notes are represented as slots and counts only;
  they do not affect score until future metric implementations are validated.
- ISSUE-0032 700F surrogate rows may appear in score output with real performance
  score `0.0` and a separate synthetic readiness score; they are not real performance
  evidence until later implementation issues produce waveform-capable runs.

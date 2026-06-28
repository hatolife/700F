# Limitations

- ISSUE-0024 scoring is an interim deterministic comparison policy, not a final M2
  downselect.
- ASR WER, STOI, ESTOI, and subjective notes are represented as slots and counts only;
  they do not affect score until future metric implementations are validated.
- Profile-only 700F candidates may appear in score output with score `0.0`; they are
  not performance evidence until later implementation issues produce completed runs.

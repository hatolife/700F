# Limitations

- ISSUE-0024 scoring is an interim deterministic comparison policy, not a final M2
  downselect.
- ASR WER, STOI, ESTOI, and subjective notes are represented as slots and counts only;
  they do not affect score until future metric implementations are validated.
- ISSUE-0032 700F surrogate rows may appear in score output with real performance
  score `0.0` and a separate synthetic readiness score; they are not real performance
  evidence until later implementation issues produce waveform-capable runs.
- ISSUE-0044 waveform and real modem prototype rows may appear with prototype
  diagnostics, but they remain excluded from real performance scoring and cannot
  enable downselect until a later audit authorizes performance-valid evidence.
- ISSUE-0047 occupied bandwidth estimates are simple deterministic DFT
  diagnostics for simulation regression. They are not regulatory measurements
  and do not make prototype rows performance-valid or downselect-valid.

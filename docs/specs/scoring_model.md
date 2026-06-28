# M2 Scoring Model v0.2.0

ISSUE-0024 defines an interim deterministic comparison policy for M2 sweep and result
records. It is a comparison aid only; it does not declare a final 700F downselect.

## Inputs

The scorer consumes Module 12 `ResultArtifact` records and optional
`ModeDescriptorSnapshot` profile snapshots. Existing metrics fields remain the source
of truth:

- completion state is inferred from `skipped_reason` and `error_summary`;
- dropout uses `dropout_rate`;
- latency uses `latency_estimate_s`;
- RF bandwidth uses `mode_descriptor.rf_bandwidth_hz`;
- BER/FER availability uses `ber`, `fer`, `supports_bit_payload`, and
  `performance_valid`;
- ISSUE-0032 surrogate readiness uses optional metrics
  `surrogate_readiness_score_synthetic` and `synthetic_metrics_label`;
- future ASR WER, STOI, ESTOI, and subjective notes use `optional_metrics` keys
  `asr_wer`, `stoi`, `estoi`, and `subjective_note`.

Skipped official baselines are counted as official-unavailable when
`mode_descriptor.official_baseline == true`. This is expected with
`F700F_ENABLE_CODEC2=OFF`; the record remains visible but is not treated as completed
performance evidence.

## Status Policy

Each result record has one inferred scoring status:

| Status | Condition | Evidence |
|---|---|---|
| completed | no `skipped_reason` and no `error_summary` | completed performance evidence |
| skipped | `skipped_reason` present | availability/configuration evidence only |
| failed | `error_summary` present and not skipped | attempted-run evidence with penalty |

700F candidates are represented by snapshots whose `implementation_status` is
`surrogate`. A surrogate snapshot can appear in score output without any result
records. Such entries have real performance score `0.0`; completed surrogate rows
increase surrogate/readiness counters but do not contribute to real performance score,
BER/FER availability, or real downselect feasibility.

ISSUE-0033 `freedv700d_emulated` and `freedv700e_emulated` rows may be operationally
completed with `implementation_status = emulated_surrogate`. These rows are
counted as completed run records for sweep health, but `performance_valid=false`
and `downselect_valid=false` mean they do not contribute completed performance
evidence to the interim score.

## Interim Score

The default policy id is `m2-interim-v0`.

For each mode, the scorer computes:

- completed run ratio;
- completed, failed, skipped, official-unavailable, and profile-only counts;
- surrogate count, performance-valid count, performance-invalid count;
- BER/FER available, unavailable, and audio-only N/A counts;
- future metric slot counts for ASR WER, STOI, ESTOI, and subjective notes;
- mean dropout rate, mean latency, max RF bandwidth;
- skipped, failed, dropout, latency, and bandwidth penalties;
- a clamped score in `[0.0, 100.0]`.
- a separate surrogate readiness score when synthetic surrogate metrics are present.

The scoring constants are intentionally simple and deterministic:

| Policy field | Default |
|---|---:|
| target RF bandwidth | 1900 Hz |
| latency reference | 0.20 s |
| completed run weight | 100.0 |
| failed attempted-run evidence weight | 55.0 |
| skipped penalty | 20.0 |
| failed penalty | 20.0 |
| dropout penalty weight | 20.0 |
| latency penalty per second over reference | 20.0 |
| bandwidth penalty per target-bandwidth overage ratio | 15.0 |

The formula is:

```text
evidence = completed_ratio * completed_run_weight
         + failed_ratio * failed_run_evidence_weight

score = clamp_0_100(evidence
                    - skipped_penalty_total
                    - failed_penalty_total
                    - dropout_penalty
                    - latency_penalty
                    - bandwidth_penalty)
```

This rewards completed real-performance runs, keeps failed attempts distinguishable
from unavailable records, and prevents skipped/profile-only/surrogate entries from
masquerading as performance evidence.

ISSUE-0032 uses real performance score as the primary `score`. Completed surrogate
rows with `performance_valid = false` receive real performance score `0.0` and may
carry a separate `surrogate_readiness_score`; that readiness score is synthetic and
must not be used as RF/audio performance evidence.

Rows with `implementation_status = emulated_surrogate` or
`optional_metrics.performance_valid = "false"` are scored with zero completed-run
evidence even when their sweep status is completed.

## Comparison Policy

Reports should sort by score descending with `mode_id` as the deterministic tie-breaker.
Human reports must display counters and penalties next to the score so a lower score can
be traced to availability, failure, dropout, latency, or bandwidth.

Human reports must also display surrogate warnings and keep real performance score
separate from synthetic surrogate readiness score.

SSB/audio-only modes may legitimately carry BER/FER as `N/A`; this is counted as
audio-only N/A rather than an unavailable digital metric. Digital modes with bit-payload
support and missing BER/FER are counted as unavailable.

## Limitations

ASR WER, STOI, ESTOI, PESQ/POLQA, MOS, and subjective notes are represented as slots
only. The interim scorer records their presence but does not weight them until validated
metric implementations are available.

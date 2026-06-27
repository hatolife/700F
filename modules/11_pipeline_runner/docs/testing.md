# Testing

Module 11 is covered by `simulation_pipeline_contract`, built from
`tests/test_simulation_pipeline.cpp`.

The test covers:

- dummy mode plus identity channel success;
- deterministic same-seed result digest, decoded audio, and metrics;
- invalid mode id failure;
- invalid channel config failure;
- zero-length generated audio;
- result metadata for run id, seed, mode id, and channel id;
- reflected metrics-stage failure.

Full local validation is `bash ./tools/run_ci_local.sh`.

# Result Artifacts v0.1.0

This document defines the baseline JSON and CSV result contract used by
`module 12 (metrics scoring)` and reporting consumers.

## Scope

This format is intentionally minimal and must remain append-only:
existing keys must stay stable; new keys can be added to
`optional_metrics` without breaking parser compatibility.

## Required top-level JSON fields

| Field | Type | Description |
|---|---|---|
| `project_version` | string | Project version label, e.g. `v0.1.0`. |
| `module_version` | string | Module 12 version label, e.g. `v0.0.1`. |
| `run_id` | string | Run identifier for the scenario or sweep. |
| `mode_descriptor` | object | Snapshot of mode metadata at runtime. |
| `mode_id` | string | Canonical mode id (copied from `mode_descriptor.mode_id`). |
| `channel_id` | string | Channel identifier used for the run. |
| `channel_parameters` | object | Channel parameter snapshot (`string` map). |
| `seed` | number | Simulation/stream seed used for repeatability. |
| `snr_db` | number | SNR setting for the run. |
| `freq_offset_hz` | number | Frequency offset in hertz. |
| `frame_count` | number | Number of frames evaluated. |
| `sample_count` | number | Number of audio samples processed. |
| `ber` | number or string | BER if computed, `"N/A"` if not applicable. |
| `fer` | number or string | FER if computed, `"N/A"` if not applicable. |
| `sync_loss_count` | number | Number of detected sync losses. |
| `dropout_rate` | number | Dropout fraction in \[0.0, 1.0\]. |
| `latency_estimate_s` | number | End-to-end latency estimate in seconds. |
| `audio_export_path` | string | Where generated audio was exported. |
| `warnings` | array\<string\> | Runtime warnings generated for this run. |
| `skipped_reason` | string \| null | Human reason when the run was skipped. |
| `error_summary` | string \| null | Human summary if the run ended in error. |
| `optional_metrics` | object | Extendable metrics dictionary (`string` values). |

### `mode_descriptor` object

| Field | Type | Description |
|---|---|---|
| `mode_id` | string | Duplicate of run-level `mode_id`. |
| `display_name` | string | Human-readable mode name. |
| `sample_rate_hz` | number | Audio sample rate. |
| `rf_bandwidth_hz` | number | RF bandwidth estimate. |
| `audio_low_hz` | number | Nominal audio low cut-off. |
| `audio_high_hz` | number | Nominal audio high cut-off. |
| `nominal_latency_s` | number | Nominal codec/modem latency. |
| `frame_duration_s` | number | Frame duration for metrics slicing. |
| `raw_bitrate_bps` | number | Raw bitrate. |
| `voice_bitrate_bps` | number | Voice bitrate. |
| `codec_id` | string | Codec identifier. |
| `fec_id` | string | FEC identifier. |
| `modem_id` | string | Modem identifier. |
| `supports_audio_input` | boolean | Whether mode accepts audio input. |
| `supports_audio_output` | boolean | Whether mode produces audio output. |
| `supports_complex_input` | boolean | Whether mode accepts complex input. |
| `supports_complex_output` | boolean | Whether mode emits complex symbols. |
| `supports_bit_payload` | boolean | Whether payload bits are available. |

## Required CSV columns (minimum)

`project_version,module_version,run_id,mode_id,mode_descriptor_snapshot,channel_id,channel_parameters,seed,snr_db,freq_offset_hz,frame_count,sample_count,ber,fer,sync_loss_count,dropout_rate,latency_estimate_s,audio_export_path,warnings,skipped_reason,error_summary`

- Unknown/optional metrics appear in additional columns with names prefixed by
  `opt.<metric_name>`.
- Values in `warnings`, `channel_parameters`, and `mode_descriptor_snapshot` are
  serialized in CSV-safe quoted strings.

## Example JSON payload

```json
{
  "project_version": "v0.1.0",
  "module_version": "v0.0.1",
  "run_id": "run-ssb-001",
  "mode_descriptor": {
    "mode_id": "ssb.reference",
    "display_name": "SSB-like reference",
    "sample_rate_hz": 8000,
    "rf_bandwidth_hz": 2700.0,
    "audio_low_hz": 300.0,
    "audio_high_hz": 3000.0,
    "nominal_latency_s": 0.0,
    "frame_duration_s": 0.04,
    "raw_bitrate_bps": 0,
    "voice_bitrate_bps": 0,
    "codec_id": "analog-voice",
    "fec_id": "none",
    "modem_id": "ssb",
    "supports_audio_input": true,
    "supports_audio_output": true,
    "supports_complex_input": false,
    "supports_complex_output": true,
    "supports_bit_payload": false
  },
  "mode_id": "ssb.reference",
  "channel_id": "awgn",
  "channel_parameters": {
    "snr_db": "12.0"
  },
  "seed": 1234,
  "snr_db": 12.0,
  "freq_offset_hz": 0.0,
  "frame_count": 16,
  "sample_count": 128000,
  "ber": "N/A",
  "fer": "N/A",
  "sync_loss_count": 0,
  "dropout_rate": 0.0,
  "latency_estimate_s": 0.1,
  "audio_export_path": "/tmp/ssb.wav",
  "warnings": [],
  "skipped_reason": null,
  "error_summary": null,
  "optional_metrics": {
    "asr_wer": "N/A"
  }
}
```

## Parsing behavior

- `ber` and `fer` values use string `"N/A"` for audio-only modes.
- Unknown keys in `optional_metrics` are ignored by older parsers and preserved by
  schema-aware readers.
- Optional future metrics (`asr_wer`, `stoi`, `estoi`, `pesq`, `polqa`, `mos`, ...)
  are expected to be placed in `optional_metrics` as string-typed entries until
  the schema formalizes their dedicated fields.

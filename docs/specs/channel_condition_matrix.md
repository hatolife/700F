# M2 Channel Condition Matrix v0.2.0

ISSUE-0025 defines the M2 channel condition matrix as reusable sweep-runner
configuration. It uses the existing Module 08 channel primitives and the existing
Module 13 expansion model:

`modes x channel_conditions x seeds`

ISSUE-0025 does not redefine channel physics, candidate implementations, scoring, or the
M2 sweep campaign. ISSUE-0026 owns campaign-specific mode selection and report cadence.

## Smoke Matrix

The smoke matrix is the CI-sized subset:

| Condition id | Channel chain | Parameters |
|---|---|---|
| `identity` | `identity` | none |
| `awgn-snr-6db` | `awgn` | `snr_db=6.0` |
| `awgn-snr-0db` | `awgn` | `snr_db=0.0` |

Seeds: `1`

The C++ helper is `f700f::make_m2_channel_matrix_smoke_sweep_config()`. The TOML-friendly
shape is recorded in `configs/channels/m2_channel_matrix_smoke.toml`.

## Full Matrix

The full matrix is a Cartesian product:

| Axis | Values |
|---|---|
| AWGN SNR dB | `-2`, `0`, `2`, `4`, `6`, `8` |
| Frequency offset Hz | `0`, `50`, `100`, `200` |
| Simple fading | `none`, `weak`, `medium` |
| Seeds | `1`, `2`, `3` |

The generated condition ids use:

`awgn-snr-<snr>db-fo-<offset>hz-fading-<profile>`

For example:

- `awgn-snr--2db-fo-0hz-fading-none`
- `awgn-snr-8db-fo-200hz-fading-medium`

Each full condition always includes `awgn` and `frequency_offset` channel entries so the
zero-offset case remains explicit. `weak` fading appends `simple_gain_fading` with
`min_gain_db=-1.5` and `max_gain_db=1.5`. `medium` fading appends `simple_gain_fading`
with `min_gain_db=-3.0` and `max_gain_db=3.0`. `none` does not append a fading channel.

This yields 72 unique channel conditions and 216 channel/seed combinations before mode
selection.

The C++ helper is `f700f::make_m2_channel_matrix_full_sweep_config()`. The TOML-friendly
axis shape is recorded in `configs/channels/m2_channel_matrix_full.toml`.

## Validation

The sweep runner rejects:

- duplicate `condition_id` values before any runs start;
- an empty seed list before any runs start;
- non-numeric or non-finite `snr_db`, `freq_offset_hz`, and fading gain values during
  channel configuration;
- negative `freq_offset_hz` values in sweep channel configs.

These checks are intentionally orchestration-level checks. Module 08 remains the source
of truth for how AWGN, frequency offset, and simple gain fading transform samples.

# Codec2/FreeDV Dependency

ISSUE-0007 pins the upstream Codec2/FreeDV source as a git submodule:

- Path: `external/codec2`
- Upstream: `https://github.com/drowe67/codec2.git`
- Pinned commit: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Pinned description: `1.2.0-108-g310777b`

The repository default remains independent of this submodule. `F700F_ENABLE_CODEC2`
defaults to `OFF`, and the normal bootstrap/local CI path must configure and build
even when `external/codec2` is absent or uninitialized. Setting
`F700F_ENABLE_CODEC2=ON` requires an initialized submodule, imports the pinned
upstream `codec2` runtime, and enables official FreeDV 700D/700E Mode-boundary
smoke regression. It does not implement any real 700F waveform candidate.

Validation:

```bash
git submodule status --recursive
python3 tools/check_submodules.py
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=OFF
```

Codec2-on local regression:

```bash
git submodule update --init --recursive external/codec2
bash ./tools/run_codec2_on_ci_local.sh
```

The Codec2-on wrapper uses a separate `build-codec2` directory, runs CTest, runs
the official FreeDV 700D/700E smoke sweep, validates official completion metadata,
and writes a comparison report. If submodule initialization fails, it exits with a
clear dependency error and does not affect default Codec2-off CI.

Authoritative baseline source notes are recorded in `docs/codec2_700d_700e_verification.md`. Values copied from websites, blog posts, or secondary summaries are not authoritative for implementation unless they are reconciled with the pinned source tree above.

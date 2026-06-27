# Codec2/FreeDV Dependency

ISSUE-0007 pins the upstream Codec2/FreeDV source as a git submodule:

- Path: `external/codec2`
- Upstream: `https://github.com/drowe67/codec2.git`
- Pinned commit: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962`
- Pinned description: `1.2.0-108-g310777b`

The repository default remains independent of this submodule. `F700F_ENABLE_CODEC2` defaults to `OFF`, and the normal bootstrap/local CI path must configure and build even when `external/codec2` is absent or uninitialized. Setting `F700F_ENABLE_CODEC2=ON` requires an initialized submodule and exposes the pinned FreeDV headers for future adapter work; it does not implement the official adapter or any 700D/700E/700F mode.

Validation:

```bash
git submodule status --recursive
python3 tools/check_submodules.py
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=OFF
```

Authoritative baseline source notes are recorded in `docs/codec2_700d_700e_verification.md`. Values copied from websites, blog posts, or secondary summaries are not authoritative for implementation unless they are reconciled with the pinned source tree above.

# ISSUE-0007 Handoff

Branch: `research/ISSUE-0007-codec2-submodule-pin`

Worktree: `C:/Users/user/Documents/f700f-wt-issue-0007`

Summary:

- Added Codec2/FreeDV as `external/codec2` submodule from `https://github.com/drowe67/codec2.git`.
- Pinned commit: `310777b1c6f1af0bc7c72f5b32f80f6fd9136962` (`1.2.0-108-g310777b`).
- Added `F700F_ENABLE_CODEC2`, default `OFF`, with an initialized-submodule check only when enabled.
- Added `tools/check_submodules.py` to validate `.gitmodules`, the `external/codec2` gitlink, and initialized worktree HEAD when present.
- Added dependency and 700D/700E verification docs with pinned source file/symbol anchors.

Verification:

- `python3 tools/check_submodules.py` passed.
- `git submodule status --recursive` reported the pinned Codec2 commit.
- `bash -lc "cmake -S . -B build-codec2-on -DCMAKE_BUILD_TYPE=Debug -DF700F_ENABLE_CODEC2=ON"` passed.
- `bash ./tools/run_ci_local.sh` passed with Codec2 disabled by default. The build emitted WSL/Windows clock-skew warnings only.

Limitations:

- No official adapter was implemented.
- No SSB, 700D, 700E, or 700F behavior was implemented.
- No push or PR was attempted; the task requested a local commit and handoff.

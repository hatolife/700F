#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

BUILD_DIR="${F700F_CODEC2_BUILD_DIR:-build-codec2}"
ARTIFACT_DIR="${F700F_CODEC2_ARTIFACT_DIR:-artifacts/sweeps/issue0037_codec2_on}"
RUN_ID="${F700F_CODEC2_RUN_ID:-issue0037-codec2-on}"
REPORT_PATH="${F700F_CODEC2_REPORT_PATH:-reports/$(date +%Y%m%d_%H%M%S)_issue0037_codec2_on_comparison.md}"

echo "==> Checking Codec2 submodule metadata"
if [[ ! -f .gitmodules ]] || ! git config --file .gitmodules --get-regexp '^submodule\.external/codec2\.' >/dev/null; then
  echo "Codec2 submodule metadata is missing. Expected external/codec2 in .gitmodules." >&2
  exit 1
fi

echo "==> Initializing Codec2 submodule"
if ! git submodule update --init --recursive external/codec2; then
  echo "Failed to initialize external/codec2. Check network access and submodule permissions." >&2
  exit 1
fi

if [[ ! -f external/codec2/src/freedv_api.h ]]; then
  echo "external/codec2/src/freedv_api.h is missing after submodule initialization." >&2
  echo "Run: git submodule update --init --recursive external/codec2" >&2
  exit 1
fi

echo "==> Codec2 submodule"
git submodule status --recursive external/codec2

echo "==> Configuring Codec2 ON build in ${BUILD_DIR}"
cmake -S . -B "${BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_SHARED_LIBS=OFF \
  -DF700F_ENABLE_CODEC2=ON

echo "==> Building Codec2 ON tree"
cmake --build "${BUILD_DIR}" --parallel "${F700F_CODEC2_BUILD_PARALLEL:-2}"

echo "==> Running Codec2 ON CTest"
ctest --test-dir "${BUILD_DIR}" --output-on-failure

echo "==> Running Codec2 ON official smoke sweep"
rm -rf "${ARTIFACT_DIR}"
"./${BUILD_DIR}/f700f-sweep" \
  --config configs/sweeps/m2_700f_candidate_smoke.toml \
  --output-dir "${ARTIFACT_DIR}" \
  --run-id "${RUN_ID}"

SUMMARY_JSON="${ARTIFACT_DIR}/${RUN_ID}.json"
if [[ ! -f "${SUMMARY_JSON}" ]]; then
  echo "Expected smoke summary JSON was not produced: ${SUMMARY_JSON}" >&2
  exit 1
fi

echo "==> Validating official FreeDV rows"
python3 - "${SUMMARY_JSON}" <<'PY'
import json
import sys

path = sys.argv[1]
with open(path, "r", encoding="utf-8") as handle:
    data = json.load(handle)

records = data.get("records", [])
official = [row for row in records if row.get("mode_id") in {"freedv700d_official", "freedv700e_official"}]
if len(official) != 6:
    raise SystemExit(f"expected 6 official rows, found {len(official)}")

for row in official:
    if row.get("status") != "completed":
        raise SystemExit(f"official row did not complete: {row}")
    note = row.get("error_summary") or ""
    required = [
        "official_freedv_completed",
        "official=true",
        "codec2_enabled=true",
        "codec2_available=true",
        "roundtrip_available=true",
        "not_emulator=true",
        "not_surrogate=true",
    ]
    missing = [text for text in required if text not in note]
    if missing:
        raise SystemExit(f"official row missing metadata {missing}: {row}")

print(f"validated {len(official)} official Codec2 ON rows")
PY

echo "==> Generating Codec2 ON comparison report"
mkdir -p "$(dirname "${REPORT_PATH}")"
"./${BUILD_DIR}/f700f-report" --input "${SUMMARY_JSON}" --output "${REPORT_PATH}"

echo "Codec2 ON local regression passed"
echo "Summary JSON: ${SUMMARY_JSON}"
echo "Report: ${REPORT_PATH}"

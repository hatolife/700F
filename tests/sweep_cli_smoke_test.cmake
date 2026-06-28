if(NOT DEFINED F700F_SWEEP_EXE)
  message(FATAL_ERROR "F700F_SWEEP_EXE is required")
endif()

if(NOT DEFINED F700F_REPO_ROOT)
  message(FATAL_ERROR "F700F_REPO_ROOT is required")
endif()

if(NOT DEFINED F700F_OUTPUT_DIR)
  message(FATAL_ERROR "F700F_OUTPUT_DIR is required")
endif()

file(REMOVE_RECURSE "${F700F_OUTPUT_DIR}")

execute_process(
  COMMAND "${F700F_SWEEP_EXE}"
          --config "${F700F_REPO_ROOT}/configs/sweeps/m2_700f_candidate_smoke.toml"
          --output-dir "${F700F_OUTPUT_DIR}"
          --run-id cli-smoke
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr)

if(NOT result EQUAL 0)
  message(FATAL_ERROR "f700f-sweep smoke failed (${result})\n${stdout}\n${stderr}")
endif()

if(NOT EXISTS "${F700F_OUTPUT_DIR}/cli-smoke.json")
  message(FATAL_ERROR "expected aggregate JSON was not generated")
endif()

if(NOT EXISTS "${F700F_OUTPUT_DIR}/cli-smoke.csv")
  message(FATAL_ERROR "expected aggregate CSV was not generated")
endif()

file(READ "${F700F_OUTPUT_DIR}/cli-smoke.json" json)
if(NOT json MATCHES "\"run_id_prefix\": \"cli-smoke\"")
  message(FATAL_ERROR "run id override was not reflected in JSON")
endif()

if(NOT json MATCHES "official_freedv_codec2_unavailable" AND
   NOT json MATCHES "freedv700d_official")
  message(FATAL_ERROR "official modes were not recorded")
endif()

set(unknown_dir "${F700F_OUTPUT_DIR}/unknown-mode")
set(unknown_config "${F700F_OUTPUT_DIR}/unknown-mode.toml")
file(MAKE_DIRECTORY "${F700F_OUTPUT_DIR}")
file(WRITE "${unknown_config}"
  "run_id_prefix = \"cli-unknown\"\n"
  "output_directory = \"${unknown_dir}\"\n"
  "export_audio = false\n"
  "metric_ids = [\"dummy.metric\"]\n"
  "seeds = [1]\n"
  "\n"
  "[input.generated_tone]\n"
  "sample_rate_hz = 8000\n"
  "sample_count = 8\n"
  "frequency_hz = 1000.0\n"
  "amplitude = 0.25\n"
  "\n"
  "[[modes]]\n"
  "mode_id = \"unknown.mode.for-cli-skip\"\n"
  "skip_if_unavailable = true\n"
  "\n"
  "[[channel_conditions]]\n"
  "condition_id = \"identity\"\n"
  "[[channel_conditions.channel_chain]]\n"
  "channel_id = \"identity\"\n")

execute_process(
  COMMAND "${F700F_SWEEP_EXE}" --config "${unknown_config}"
  RESULT_VARIABLE unknown_result
  OUTPUT_VARIABLE unknown_stdout
  ERROR_VARIABLE unknown_stderr)

if(NOT unknown_result EQUAL 0)
  message(FATAL_ERROR
    "f700f-sweep unknown-mode smoke failed (${unknown_result})\n"
    "${unknown_stdout}\n${unknown_stderr}")
endif()

file(READ "${unknown_dir}/cli-unknown.json" unknown_json)
if(NOT unknown_json MATCHES "unknown.mode.for-cli-skip")
  message(FATAL_ERROR "unknown mode was not present in CLI output")
endif()

if(NOT unknown_json MATCHES "mode id not registered: unknown.mode.for-cli-skip")
  message(FATAL_ERROR "unknown mode was not recorded as a skip")
endif()

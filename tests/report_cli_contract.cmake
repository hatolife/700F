set(sample_json "${F700F_REPORT_TEST_DIR}/sample-sweep.json")
set(malformed_json "${F700F_REPORT_TEST_DIR}/malformed.json")
set(malformed_csv "${F700F_REPORT_TEST_DIR}/malformed.csv")
set(report_md "${F700F_REPORT_TEST_DIR}/report.md")

file(MAKE_DIRECTORY "${F700F_REPORT_TEST_DIR}")
file(WRITE "${sample_json}" [=[
{
  "ok": true,
  "run_id_prefix": "m2-700f-candidate-smoke",
  "error": "",
  "records": [
    {"run_id": "m2-ssb", "status": "completed", "mode_id": "ssb_standard_3k", "condition_id": "identity", "seed": 1, "skipped_reason": null, "error_summary": null, "simulation_digest": "abc"},
    {"run_id": "m2-official", "status": "skipped", "mode_id": "freedv700d_official", "condition_id": "identity", "seed": 1, "skipped_reason": "official_waveform_roundtrip_not_implemented", "error_summary": null, "simulation_digest": ""},
    {"run_id": "m2-profile", "status": "completed", "mode_id": "freedv700f_a_balanced", "condition_id": "awgn-snr-6db", "seed": 1, "skipped_reason": null, "error_summary": "profile_only_completed", "simulation_digest": "profile"},
    {"run_id": "m2-descriptor", "status": "completed", "mode_id": "freedv700d_emulated", "condition_id": "awgn-snr-6db", "seed": 1, "skipped_reason": null, "error_summary": "descriptor_only_completed", "simulation_digest": "descriptor"}
  ]
}
]=])
file(WRITE "${malformed_json}" "{ not json")
file(WRITE "${malformed_csv}" "run_id,status\nonly-one-field\n")

execute_process(
  COMMAND "${F700F_REPORT_EXE}" --input "${sample_json}" --output "${report_md}"
  RESULT_VARIABLE sample_result
  OUTPUT_VARIABLE sample_stdout
  ERROR_VARIABLE sample_stderr)
if(NOT sample_result EQUAL 0)
  message(FATAL_ERROR "f700f-report sample JSON failed: ${sample_stderr}")
endif()
file(READ "${report_md}" report_text)
if(NOT report_text MATCHES "M2 Baseline Comparison Report")
  message(FATAL_ERROR "sample report did not contain report heading")
endif()
if(NOT report_text MATCHES "Real downselect possible: no")
  message(FATAL_ERROR "sample report did not state downselect feasibility")
endif()
if(NOT report_text MATCHES "profile_only")
  message(FATAL_ERROR "sample report did not include profile-only rows")
endif()
if(NOT report_text MATCHES "descriptor_only")
  message(FATAL_ERROR "sample report did not include descriptor-only rows")
endif()

execute_process(
  COMMAND "${F700F_REPORT_EXE}"
  RESULT_VARIABLE missing_result
  OUTPUT_VARIABLE missing_stdout
  ERROR_VARIABLE missing_stderr)
if(missing_result EQUAL 0 OR NOT missing_stderr MATCHES "missing required --input")
  message(FATAL_ERROR "missing input did not fail clearly: ${missing_stderr}")
endif()

execute_process(
  COMMAND "${F700F_REPORT_EXE}" --input "${malformed_json}"
  RESULT_VARIABLE bad_json_result
  OUTPUT_VARIABLE bad_json_stdout
  ERROR_VARIABLE bad_json_stderr)
if(bad_json_result EQUAL 0 OR NOT bad_json_stderr MATCHES "Malformed JSON")
  message(FATAL_ERROR "malformed JSON did not fail clearly: ${bad_json_stderr}")
endif()

execute_process(
  COMMAND "${F700F_REPORT_EXE}" --input "${malformed_csv}"
  RESULT_VARIABLE bad_csv_result
  OUTPUT_VARIABLE bad_csv_stdout
  ERROR_VARIABLE bad_csv_stderr)
if(bad_csv_result EQUAL 0 OR NOT bad_csv_stderr MATCHES "Malformed CSV")
  message(FATAL_ERROR "malformed CSV did not fail clearly: ${bad_csv_stderr}")
endif()

if(NOT DEFINED F700F_SWEEP_EXE)
  message(FATAL_ERROR "F700F_SWEEP_EXE is required")
endif()

execute_process(
  COMMAND "${F700F_SWEEP_EXE}" --config build/test-artifacts/missing-sweep-config.toml
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr)

if(result EQUAL 0)
  message(FATAL_ERROR "f700f-sweep unexpectedly accepted a missing config")
endif()

set(output "${stdout}\n${stderr}")
if(NOT output MATCHES "missing config")
  message(FATAL_ERROR "missing config error was not clear:\n${output}")
endif()

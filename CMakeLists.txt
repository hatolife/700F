cmake_minimum_required(VERSION 3.25)
project(freedv700f_lab VERSION 0.2.0 LANGUAGES CXX)
option(F700F_ENABLE_CODEC2 "Enable integration points that require the external Codec2/FreeDV submodule" OFF)
set(F700F_CODEC2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/codec2" CACHE PATH
    "Path to the pinned Codec2/FreeDV source tree used when F700F_ENABLE_CODEC2=ON")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(f700f_core INTERFACE)
target_include_directories(f700f_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(f700f_candidate_profiles STATIC src/candidate_profiles.cpp)
target_link_libraries(f700f_candidate_profiles PUBLIC f700f_core)
add_library(f700f_metrics OBJECT
  modules/12_metrics_scoring/src/metrics_artifacts.cpp
  modules/12_metrics_scoring/src/occupied_bandwidth.cpp
  modules/12_metrics_scoring/src/scoring_model.cpp)
target_include_directories(f700f_metrics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include)
add_library(f700f_reporting OBJECT
  modules/15_reporting_visualization/src/baseline_comparison_report.cpp
  modules/15_reporting_visualization/src/report_artifact_loader.cpp)
target_include_directories(f700f_reporting PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/15_reporting_visualization/include)
add_subdirectory(modules/08_hf_channel_simulator)
add_subdirectory(modules/10_reference_baselines_ssb_700d_700e)
if(F700F_ENABLE_CODEC2)
  enable_language(C)
  set(F700F_CODEC2_SOURCE_DIR ${F700F_CODEC2_ROOT})
  if(NOT EXISTS ${F700F_CODEC2_SOURCE_DIR}/src/freedv_api.h)
    message(FATAL_ERROR "F700F_ENABLE_CODEC2=ON requires initialized Codec2 source at ${F700F_CODEC2_SOURCE_DIR}; run git submodule update --init --recursive external/codec2 or set F700F_CODEC2_ROOT")
  endif()
  set(LPCNET OFF CACHE BOOL "Build Codec2 with LPCNet support" FORCE)
  set(UNITTEST OFF CACHE BOOL "Build Codec2 unittest binaries" FORCE)
  message(STATUS "F700F Codec2 integration enabled: ${F700F_CODEC2_SOURCE_DIR}")
  if(DEFINED BUILD_SHARED_LIBS)
    set(F700F_SAVED_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")
    set(F700F_HAD_BUILD_SHARED_LIBS TRUE)
  else()
    set(F700F_HAD_BUILD_SHARED_LIBS FALSE)
  endif()
  add_subdirectory(${F700F_CODEC2_SOURCE_DIR}
                   ${CMAKE_CURRENT_BINARY_DIR}/external/codec2
                   EXCLUDE_FROM_ALL)
  if(F700F_HAD_BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${F700F_SAVED_BUILD_SHARED_LIBS}")
  else()
    unset(BUILD_SHARED_LIBS)
  endif()
  add_library(f700f_codec2_headers INTERFACE)
  target_include_directories(f700f_codec2_headers INTERFACE ${F700F_CODEC2_SOURCE_DIR}/src)
  if(NOT TARGET codec2)
    message(FATAL_ERROR "F700F_ENABLE_CODEC2=ON expected Codec2 target `codec2` from ${F700F_CODEC2_SOURCE_DIR}")
  endif()
  add_library(f700f_codec2_runtime INTERFACE)
  target_link_libraries(f700f_codec2_runtime INTERFACE codec2)
else()
  message(STATUS "F700F Codec2 integration disabled: F700F_ENABLE_CODEC2=OFF")
endif()
add_subdirectory(modules/05_codec_adapter)
add_executable(f700f-info src/apps/f700f-info.cpp)
target_link_libraries(f700f-info PRIVATE f700f_core)
add_executable(f700f-report
  src/apps/f700f-report.cpp
  $<TARGET_OBJECTS:f700f_metrics>
  $<TARGET_OBJECTS:f700f_reporting>)
target_link_libraries(f700f-report PRIVATE f700f_core)
target_include_directories(f700f-report PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/15_reporting_visualization/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(modules/11_pipeline_runner)
add_subdirectory(modules/13_sweep_optimization)
add_executable(f700f-sweep src/apps/f700f-sweep.cpp)
target_link_libraries(f700f-sweep PRIVATE f700f_core f700f_sweep_runner)
add_executable(test_mode_interface tests/test_mode_interface.cpp)
target_link_libraries(test_mode_interface PRIVATE f700f_core)
add_executable(test_modem_frame_symbol_protocol tests/test_modem_frame_symbol_protocol.cpp)
target_link_libraries(test_modem_frame_symbol_protocol PRIVATE f700f_core)
add_executable(test_metrics_result_artifacts
  tests/test_metrics_result_artifacts.cpp
  $<TARGET_OBJECTS:f700f_metrics>)
target_link_libraries(test_metrics_result_artifacts PRIVATE f700f_core)
target_include_directories(test_metrics_result_artifacts PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(test_occupied_bandwidth
  tests/test_occupied_bandwidth.cpp
  $<TARGET_OBJECTS:f700f_metrics>)
target_link_libraries(test_occupied_bandwidth PRIVATE f700f_core)
target_include_directories(test_occupied_bandwidth PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(test_scoring_model
  tests/test_scoring_model.cpp
  $<TARGET_OBJECTS:f700f_metrics>)
target_link_libraries(test_scoring_model PRIVATE f700f_core f700f_candidate_profiles)
target_include_directories(test_scoring_model PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(test_baseline_comparison_report
  tests/test_baseline_comparison_report.cpp
  $<TARGET_OBJECTS:f700f_metrics>
  $<TARGET_OBJECTS:f700f_reporting>)
target_link_libraries(test_baseline_comparison_report
  PRIVATE f700f_core f700f_candidate_profiles)
target_include_directories(test_baseline_comparison_report PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/15_reporting_visualization/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(test_report_artifact_loader_cli
  tests/test_report_artifact_loader_cli.cpp
  $<TARGET_OBJECTS:f700f_metrics>
  $<TARGET_OBJECTS:f700f_reporting>)
target_link_libraries(test_report_artifact_loader_cli PRIVATE f700f_core)
target_include_directories(test_report_artifact_loader_cli PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/12_metrics_scoring/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/15_reporting_visualization/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(test_channel_models tests/test_channel_models.cpp)
target_link_libraries(test_channel_models PRIVATE f700f_core f700f_channel_model)
add_executable(test_freedv_emulator tests/test_freedv_emulator.cpp)
target_link_libraries(test_freedv_emulator PRIVATE f700f_core f700f_freedv_emulator)
add_executable(test_ssb_reference_emulator tests/test_ssb_reference_emulator.cpp)
target_link_libraries(test_ssb_reference_emulator PRIVATE f700f_core f700f_freedv_emulator)
add_executable(test_freedv_official_adapter tests/test_freedv_official_adapter.cpp)
target_link_libraries(test_freedv_official_adapter
  PRIVATE
    f700f_core
    f700f_codec_adapter
    f700f_freedv_emulator)
add_executable(test_simulation_pipeline tests/test_simulation_pipeline.cpp)
target_link_libraries(test_simulation_pipeline PRIVATE f700f_core f700f_pipeline_runner f700f_freedv_emulator)
add_executable(test_sweep_runner tests/test_sweep_runner.cpp)
target_link_libraries(test_sweep_runner PRIVATE f700f_core f700f_sweep_runner)
add_executable(test_sweep_config_loader tests/test_sweep_config_loader.cpp)
target_link_libraries(test_sweep_config_loader PRIVATE f700f_core f700f_sweep_runner)
add_executable(test_700f_candidate_profiles tests/test_700f_candidate_profiles.cpp)
target_link_libraries(test_700f_candidate_profiles
  PRIVATE
    f700f_core
    f700f_candidate_profiles
    f700f_metrics
    f700f_sweep_runner)
enable_testing()
add_test(NAME f700f_info_version COMMAND f700f-info)
add_test(NAME f700f_report_help COMMAND f700f-report --help)
set_tests_properties(f700f_report_help PROPERTIES
  PASS_REGULAR_EXPRESSION "Usage: f700f-report")
add_test(NAME f700f_report_missing_input COMMAND f700f-report)
set_tests_properties(f700f_report_missing_input PROPERTIES WILL_FAIL TRUE)
add_test(NAME mode_interface_contract COMMAND test_mode_interface)
add_test(NAME modem_frame_symbol_protocol_contract COMMAND test_modem_frame_symbol_protocol)
add_test(NAME metrics_result_artifacts COMMAND test_metrics_result_artifacts)
add_test(NAME occupied_bandwidth_contract COMMAND test_occupied_bandwidth)
add_test(NAME scoring_model_contract COMMAND test_scoring_model)
add_test(NAME baseline_comparison_report_contract COMMAND test_baseline_comparison_report)
add_test(NAME report_artifact_loader_cli_contract COMMAND test_report_artifact_loader_cli)
add_test(NAME f700f_report_cli_contract
  COMMAND ${CMAKE_COMMAND}
    -DF700F_REPORT_EXE=$<TARGET_FILE:f700f-report>
    -DF700F_REPORT_TEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/test-artifacts/report-cli
    -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/report_cli_contract.cmake)
add_test(NAME channel_model_contract COMMAND test_channel_models)
add_test(NAME freedv_emulator_contract COMMAND test_freedv_emulator)
add_test(NAME ssb_reference_emulator_contract COMMAND test_ssb_reference_emulator)
add_test(NAME freedv_official_adapter_contract COMMAND test_freedv_official_adapter)
add_test(NAME simulation_pipeline_contract COMMAND test_simulation_pipeline)
add_test(NAME sweep_runner_contract COMMAND test_sweep_runner)
add_test(NAME sweep_config_loader_contract COMMAND test_sweep_config_loader)
add_test(NAME sweep_cli_help COMMAND f700f-sweep --help)
set_tests_properties(sweep_cli_help PROPERTIES
  PASS_REGULAR_EXPRESSION "Usage: f700f-sweep")
add_test(NAME sweep_cli_missing_config
  COMMAND ${CMAKE_COMMAND}
    -DF700F_SWEEP_EXE=$<TARGET_FILE:f700f-sweep>
    -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/sweep_cli_missing_config_test.cmake)
add_test(NAME sweep_cli_smoke
  COMMAND ${CMAKE_COMMAND}
    -DF700F_SWEEP_EXE=$<TARGET_FILE:f700f-sweep>
    -DF700F_REPO_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
    -DF700F_OUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}/test-artifacts/cli-smoke
    -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/sweep_cli_smoke_test.cmake)
add_test(NAME candidate_700f_profiles_contract COMMAND test_700f_candidate_profiles)

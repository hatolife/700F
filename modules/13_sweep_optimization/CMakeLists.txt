add_library(f700f_sweep_runner
  src/sweep_runner.cpp
)

target_include_directories(f700f_sweep_runner
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(f700f_sweep_runner
  PUBLIC
    f700f_core
    f700f_pipeline_runner
  PRIVATE
    f700f_channel_model
)

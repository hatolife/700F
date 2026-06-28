add_library(f700f_pipeline_runner STATIC
  src/simulation_pipeline.cpp
)

target_include_directories(f700f_pipeline_runner
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(f700f_pipeline_runner
  PUBLIC
    f700f_core
)

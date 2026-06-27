add_library(f700f_channel_model
  src/channel_model.cpp
)

target_include_directories(f700f_channel_model PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(f700f_channel_model PUBLIC f700f_core)

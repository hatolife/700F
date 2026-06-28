add_library(f700f_codec_adapter STATIC
  src/freedv_official.cpp
)

target_include_directories(f700f_codec_adapter
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(f700f_codec_adapter
  PUBLIC
    f700f_core
)

if(F700F_ENABLE_CODEC2)
  target_link_libraries(f700f_codec_adapter PRIVATE f700f_codec2_headers f700f_codec2_runtime)
  target_compile_definitions(f700f_codec_adapter PUBLIC F700F_CODEC2_AVAILABLE=1)
else()
  target_compile_definitions(f700f_codec_adapter PUBLIC F700F_CODEC2_AVAILABLE=0)
endif()

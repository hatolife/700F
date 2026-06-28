add_library(f700f_freedv_emulator STATIC
  src/freedv_emulator.cpp
)

target_include_directories(f700f_freedv_emulator
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(f700f_freedv_emulator
  PUBLIC
    f700f_core
)

add_library(f700f_ofdm_modem_waveform STATIC
  src/carrier_layout.cpp
)

target_include_directories(f700f_ofdm_modem_waveform
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(f700f_ofdm_modem_waveform
  PUBLIC
    f700f_core
)

add_executable(test_ofdm_carrier_layout
  tests/test_ofdm_carrier_layout.cpp
)

target_link_libraries(test_ofdm_carrier_layout
  PRIVATE
    f700f_ofdm_modem_waveform
    f700f_core
    f700f_candidate_profiles
)

add_test(NAME ofdm_carrier_layout_contract COMMAND test_ofdm_carrier_layout)

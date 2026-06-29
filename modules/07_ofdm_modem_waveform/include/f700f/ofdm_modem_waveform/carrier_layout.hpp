#pragma once

#include <f700f/types.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace f700f::ofdm_modem_waveform {

enum class CarrierRole {
  data,
  pilot,
  guard,
};

struct PrototypeCarrier {
  std::int32_t index = 0;
  double frequency_offset_hz = 0.0;
  CarrierRole role = CarrierRole::data;
};

struct OfdmCarrierLayoutValidation {
  bool valid = false;
  std::vector<std::string> errors;
};

struct OfdmCarrierLayoutDescriptor {
  std::uint32_t carrier_count = 0;
  std::uint32_t data_carrier_count = 0;
  std::uint32_t pilot_carrier_count = 0;
  std::uint32_t guard_carrier_count = 0;
  double carrier_spacing_hz = 0.0;
  double occupied_bandwidth_target_hz = 0.0;
  double center_frequency_hz = 0.0;
  std::string modem_family = "ofdm_candidate";
  std::string fec_family = "placeholder";
  std::string sync_family = "pilot_placeholder";
  std::string codec_family = "synthetic";
  bool prototype = true;
  bool not_final_modem = true;
  bool downselect_valid = false;
  std::string performance_validity = "limited";
  std::vector<PrototypeCarrier> carriers;
  std::vector<std::string> limitations;
};

OfdmCarrierLayoutDescriptor make_700f_a_ofdm_carrier_layout_prototype();

OfdmCarrierLayoutValidation
validate_ofdm_carrier_layout(const OfdmCarrierLayoutDescriptor &layout);

std::uint32_t count_carriers_with_role(
    const OfdmCarrierLayoutDescriptor &layout, CarrierRole role) noexcept;

BasebandFrame make_700f_a_ofdm_baseband_frame_prototype(
    const OfdmCarrierLayoutDescriptor &layout, SampleRate sample_rate,
    SymbolRate symbol_rate);

} // namespace f700f::ofdm_modem_waveform

#include <f700f/candidate_profiles.hpp>
#include <f700f/ofdm_modem_waveform/carrier_layout.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace {

using f700f::ofdm_modem_waveform::CarrierRole;
using f700f::ofdm_modem_waveform::count_carriers_with_role;
using f700f::ofdm_modem_waveform::make_700f_a_ofdm_baseband_frame_prototype;
using f700f::ofdm_modem_waveform::make_700f_a_ofdm_carrier_layout_prototype;
using f700f::ofdm_modem_waveform::validate_ofdm_carrier_layout;

bool contains_fragment(const std::vector<std::string> &values,
                       const std::string &fragment) {
  return std::any_of(values.begin(), values.end(), [&](const auto &value) {
    return value.find(fragment) != std::string::npos;
  });
}

void default_layout_records_prototype_metadata() {
  const auto layout = make_700f_a_ofdm_carrier_layout_prototype();
  const auto validation = validate_ofdm_carrier_layout(layout);

  assert(validation.valid);
  assert(layout.carrier_count == 19);
  assert(layout.carrier_spacing_hz == 100.0);
  assert(layout.occupied_bandwidth_target_hz == 1900.0);
  assert(layout.center_frequency_hz == 1500.0);
  assert(layout.data_carrier_count == 15);
  assert(layout.pilot_carrier_count == 2);
  assert(layout.guard_carrier_count == 2);
  assert(layout.carriers.size() == layout.carrier_count);
  assert(count_carriers_with_role(layout, CarrierRole::data) ==
         layout.data_carrier_count);
  assert(count_carriers_with_role(layout, CarrierRole::pilot) ==
         layout.pilot_carrier_count);
  assert(count_carriers_with_role(layout, CarrierRole::guard) ==
         layout.guard_carrier_count);
  assert(layout.modem_family == "ofdm_candidate");
  assert(layout.fec_family == "placeholder");
  assert(layout.sync_family == "pilot_placeholder");
  assert(layout.prototype);
  assert(layout.not_final_modem);
  assert(!layout.downselect_valid);
  assert(layout.performance_validity == "limited");
  assert(!layout.limitations.empty());
}

void invalid_layouts_report_reasons() {
  auto layout = make_700f_a_ofdm_carrier_layout_prototype();

  layout.carrier_spacing_hz = 0.0;
  auto validation = validate_ofdm_carrier_layout(layout);
  assert(!validation.valid);
  assert(contains_fragment(validation.errors, "carrier_spacing_hz"));

  layout = make_700f_a_ofdm_carrier_layout_prototype();
  layout.occupied_bandwidth_target_hz = -1.0;
  validation = validate_ofdm_carrier_layout(layout);
  assert(!validation.valid);
  assert(contains_fragment(validation.errors, "occupied_bandwidth_target_hz"));

  layout = make_700f_a_ofdm_carrier_layout_prototype();
  layout.carrier_count = 0;
  layout.carriers.clear();
  validation = validate_ofdm_carrier_layout(layout);
  assert(!validation.valid);
  assert(contains_fragment(validation.errors, "carrier_count"));

  layout = make_700f_a_ofdm_carrier_layout_prototype();
  layout.guard_carrier_count = 3;
  validation = validate_ofdm_carrier_layout(layout);
  assert(!validation.valid);
  assert(contains_fragment(validation.errors, "data+pilot+guard"));
}

void baseband_frame_carries_layout_placeholders() {
  const auto layout = make_700f_a_ofdm_carrier_layout_prototype();
  const auto frame = make_700f_a_ofdm_baseband_frame_prototype(
      layout, f700f::SampleRate{.hz = 8000},
      f700f::SymbolRate{.baud = 1200.0});

  assert(frame.samples.empty());
  assert(frame.sample_rate.hz == 8000);
  assert(frame.symbol_rate.baud == 1200.0);
  assert(frame.occupied_bandwidth_hz == layout.occupied_bandwidth_target_hz);
  assert(frame.center_frequency_hz == layout.center_frequency_hz);
  assert(frame.modem_family == "ofdm_candidate");
  assert(frame.sync_family == "pilot_placeholder");
  assert(frame.descriptor.mode_id == "freedv700f_a_balanced");
  assert(frame.descriptor.valid_rates());
  assert(frame.descriptor.modem_family == "ofdm_candidate");
  assert(frame.descriptor.carrier_layout.family ==
         "ofdm_carrier_layout_placeholder");
  assert(frame.descriptor.carrier_layout.carrier_count == layout.carrier_count);
  assert(frame.descriptor.carrier_layout.occupied_bandwidth_hz == 1900.0);
  assert(frame.descriptor.fec.family == "placeholder");
  assert(!frame.descriptor.fec.enabled);
  assert(frame.descriptor.sync.family == "pilot_placeholder");
  assert(!frame.descriptor.sync.locked);
  assert(frame.descriptor.codec.family == "synthetic");
  assert(!frame.descriptor.limitations.empty());
}

void minimal_qpsk_runtime_metadata_is_preserved() {
  f700f::AudioBlock audio;
  audio.sample_rate_hz = 8000;
  audio.mono = {0.1F, -0.2F, 0.0F, 0.3F};

  const auto input = f700f::make_700f_a_minimal_qpsk_input_frame(audio, 4, 46);
  const auto symbols = f700f::encode_700f_a_minimal_qpsk_symbols(input);
  const auto baseband = f700f::modulate_700f_a_minimal_qpsk_baseband(symbols);

  assert(input.descriptor.modem_family == "minimal_qpsk");
  assert(input.requested_fec_family == "none");
  assert(input.requested_sync_family == "none");
  assert(symbols.modem_family == "minimal_qpsk");
  assert(symbols.fec_family == "none");
  assert(symbols.sync_family == "none");
  assert(baseband.modem_family == "minimal_qpsk");
  assert(baseband.sync_family == "none");
  assert(baseband.descriptor.carrier_layout.family ==
         "minimal_qpsk_single_carrier");
  assert(!baseband.descriptor.fec.enabled);
  assert(!baseband.descriptor.sync.locked);
}

} // namespace

int main() {
  default_layout_records_prototype_metadata();
  invalid_layouts_report_reasons();
  baseband_frame_carries_layout_placeholders();
  minimal_qpsk_runtime_metadata_is_preserved();
  return 0;
}

#include <f700f/ofdm_modem_waveform/carrier_layout.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

namespace f700f::ofdm_modem_waveform {
namespace {

constexpr std::uint32_t kCarrierCount = 19;
constexpr std::uint32_t kDataCarrierCount = 15;
constexpr std::uint32_t kPilotCarrierCount = 2;
constexpr std::uint32_t kGuardCarrierCount = 2;
constexpr double kCarrierSpacingHz = 100.0;
constexpr double kOccupiedBandwidthTargetHz = 1900.0;
constexpr double kCenterFrequencyHz = 1500.0;

std::vector<std::string> prototype_limitations() {
  return {
      "ISSUE-0046 prototype metadata only; no OFDM modulation is implemented",
      "ISSUE-0046 FEC placeholder only; no error correction is applied",
      "ISSUE-0046 sync placeholder only; no timing or carrier recovery is "
      "implemented",
      "real 700F downselect remains prohibited",
  };
}

CarrierRole role_for_index(std::int32_t index) noexcept {
  if (index == -9 || index == 9) {
    return CarrierRole::guard;
  }
  if (index == -3 || index == 3) {
    return CarrierRole::pilot;
  }
  return CarrierRole::data;
}

std::vector<PrototypeCarrier> make_carriers() {
  std::vector<PrototypeCarrier> carriers;
  carriers.reserve(kCarrierCount);
  for (std::int32_t index = -9; index <= 9; ++index) {
    carriers.push_back(PrototypeCarrier{
        .index = index,
        .frequency_offset_hz = static_cast<double>(index) * kCarrierSpacingHz,
        .role = role_for_index(index),
    });
  }
  return carriers;
}

void append_validation_limitations(BasebandFrame &frame,
                                   const OfdmCarrierLayoutValidation &result) {
  if (result.valid) {
    return;
  }
  for (const auto &error : result.errors) {
    frame.limitations.push_back("invalid OFDM carrier layout: " + error);
    frame.descriptor.limitations.push_back("invalid OFDM carrier layout: " +
                                           error);
  }
}

} // namespace

OfdmCarrierLayoutDescriptor make_700f_a_ofdm_carrier_layout_prototype() {
  OfdmCarrierLayoutDescriptor layout;
  layout.carrier_count = kCarrierCount;
  layout.data_carrier_count = kDataCarrierCount;
  layout.pilot_carrier_count = kPilotCarrierCount;
  layout.guard_carrier_count = kGuardCarrierCount;
  layout.carrier_spacing_hz = kCarrierSpacingHz;
  layout.occupied_bandwidth_target_hz = kOccupiedBandwidthTargetHz;
  layout.center_frequency_hz = kCenterFrequencyHz;
  layout.carriers = make_carriers();
  layout.limitations = prototype_limitations();
  return layout;
}

OfdmCarrierLayoutValidation
validate_ofdm_carrier_layout(const OfdmCarrierLayoutDescriptor &layout) {
  OfdmCarrierLayoutValidation result;

  if (layout.carrier_count == 0) {
    result.errors.emplace_back("carrier_count must be greater than zero");
  }
  if (!std::isfinite(layout.carrier_spacing_hz) ||
      layout.carrier_spacing_hz <= 0.0) {
    result.errors.emplace_back("carrier_spacing_hz must be positive");
  }
  if (!std::isfinite(layout.occupied_bandwidth_target_hz) ||
      layout.occupied_bandwidth_target_hz <= 0.0) {
    result.errors.emplace_back(
        "occupied_bandwidth_target_hz must be positive");
  }
  if (!std::isfinite(layout.center_frequency_hz)) {
    result.errors.emplace_back("center_frequency_hz must be finite");
  }

  const auto role_sum = layout.data_carrier_count +
                        layout.pilot_carrier_count +
                        layout.guard_carrier_count;
  if (role_sum != layout.carrier_count) {
    result.errors.emplace_back(
        "data+pilot+guard carrier counts must equal carrier_count");
  }

  if (layout.carriers.size() != layout.carrier_count) {
    result.errors.emplace_back("carriers size must equal carrier_count");
  } else {
    if (count_carriers_with_role(layout, CarrierRole::data) !=
        layout.data_carrier_count) {
      result.errors.emplace_back(
          "data carrier role count must equal data_carrier_count");
    }
    if (count_carriers_with_role(layout, CarrierRole::pilot) !=
        layout.pilot_carrier_count) {
      result.errors.emplace_back(
          "pilot carrier role count must equal pilot_carrier_count");
    }
    if (count_carriers_with_role(layout, CarrierRole::guard) !=
        layout.guard_carrier_count) {
      result.errors.emplace_back(
          "guard carrier role count must equal guard_carrier_count");
    }
  }

  result.valid = result.errors.empty();
  return result;
}

std::uint32_t count_carriers_with_role(
    const OfdmCarrierLayoutDescriptor &layout, CarrierRole role) noexcept {
  return static_cast<std::uint32_t>(std::count_if(
      layout.carriers.begin(), layout.carriers.end(),
      [role](const auto &carrier) { return carrier.role == role; }));
}

BasebandFrame make_700f_a_ofdm_baseband_frame_prototype(
    const OfdmCarrierLayoutDescriptor &layout, SampleRate sample_rate,
    SymbolRate symbol_rate) {
  BasebandFrame frame;
  frame.sample_rate = sample_rate;
  frame.symbol_rate = symbol_rate;
  frame.occupied_bandwidth_hz = layout.occupied_bandwidth_target_hz;
  frame.center_frequency_hz = layout.center_frequency_hz;
  frame.modem_family = layout.modem_family;
  frame.sync_family = layout.sync_family;
  frame.limitations = layout.limitations;

  frame.descriptor.mode_id = "freedv700f_a_balanced";
  frame.descriptor.sample_rate = sample_rate;
  frame.descriptor.symbol_rate = symbol_rate;
  frame.descriptor.modem_family = layout.modem_family;
  frame.descriptor.carrier_layout.family = "ofdm_carrier_layout_placeholder";
  frame.descriptor.carrier_layout.carrier_count = layout.carrier_count;
  frame.descriptor.carrier_layout.center_frequency_hz =
      layout.center_frequency_hz;
  frame.descriptor.carrier_layout.occupied_bandwidth_hz =
      layout.occupied_bandwidth_target_hz;
  frame.descriptor.carrier_layout.limitations = layout.limitations;
  frame.descriptor.fec.family = layout.fec_family;
  frame.descriptor.fec.enabled = false;
  frame.descriptor.sync.family = layout.sync_family;
  frame.descriptor.sync.locked = false;
  frame.descriptor.codec.family = layout.codec_family;
  frame.descriptor.codec.encoded = false;
  frame.descriptor.codec.decoded = false;
  frame.descriptor.limitations = layout.limitations;

  append_validation_limitations(frame, validate_ofdm_carrier_layout(layout));
  return frame;
}

} // namespace f700f::ofdm_modem_waveform

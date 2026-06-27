#include <f700f/reference_baselines/freedv_emulator.hpp>
#include <f700f/types.hpp>

#include <cassert>
#include <memory>
#include <string>

namespace {

void assert_descriptor_shape(const f700f::ModeDescriptor &descriptor,
                             const std::string &mode_id) {
  assert(descriptor.mode_id == mode_id);
  assert(!descriptor.display_name.empty());
  assert(descriptor.rf_bandwidth_hz > 0.0);
  assert(descriptor.audio_high_hz > descriptor.audio_low_hz);
  assert(descriptor.frame_duration_s > 0.0);
  assert(descriptor.cp_duration_s > 0.0);
  assert(descriptor.raw_bitrate_bps > 0);
  assert(descriptor.carrier_count > 0);
  assert(!descriptor.modulation_family.empty());
  assert(!descriptor.fec_id.empty());
  assert(!descriptor.pilot_strategy.empty());
  assert(!descriptor.implementation_status.empty());
  assert(descriptor.emulator);
  assert(!descriptor.official_baseline);
}

void descriptors_are_obtainable_test() {
  assert_descriptor_shape(f700f::freedv700d_emulated_descriptor(),
                          "freedv700d_emulated");
  assert_descriptor_shape(f700f::freedv700e_emulated_descriptor(),
                          "freedv700e_emulated");
}

void registry_selects_both_modes_test() {
  f700f::ModeRegistry registry;
  f700f::register_freedv_emulator_modes(registry);

  const auto *mode_700d = registry.find("freedv700d_emulated");
  const auto *mode_700e = registry.find("freedv700e_emulated");
  assert(mode_700d != nullptr);
  assert(mode_700e != nullptr);
  assert(mode_700d->emulator);
  assert(mode_700e->emulator);
  assert(!mode_700d->official_baseline);
  assert(!mode_700e->official_baseline);
}

void encode_decode_report_unimplemented_test() {
  f700f::ModeRegistry registry;
  f700f::register_freedv_emulator_modes(registry);
  auto mode = registry.create("freedv700d_emulated");
  assert(mode != nullptr);
  assert(mode->configure({.sample_rate_hz = mode->descriptor().sample_rate_hz}));

  const auto encoded = mode->encode({});
  assert(!encoded.ok);
  assert(encoded.error.find("ISSUE-0013") != std::string::npos);
  assert(encoded.error.find("not implemented") != std::string::npos);

  const auto decoded = mode->decode({});
  assert(!decoded.ok);
  assert(decoded.error.find("ISSUE-0013") != std::string::npos);
  assert(decoded.error.find("not implemented") != std::string::npos);
}

} // namespace

int main() {
  descriptors_are_obtainable_test();
  registry_selects_both_modes_test();
  encode_decode_report_unimplemented_test();
  return 0;
}

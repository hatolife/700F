#include <f700f/codec_adapter/freedv_official.hpp>
#include <f700f/reference_baselines/freedv_emulator.hpp>
#include <f700f/types.hpp>

#include <cassert>
#include <memory>
#include <string>

namespace {

bool contains(const std::string &haystack, const std::string &needle) {
  return haystack.find(needle) != std::string::npos;
}

void assert_official_descriptor_shape(const f700f::ModeDescriptor &descriptor,
                                      const std::string &mode_id,
                                      int codec2_mode_id) {
  assert(descriptor.mode_id == mode_id);
  assert(!descriptor.display_name.empty());
  assert(descriptor.sample_rate_hz == 8000);
  assert(descriptor.rf_bandwidth_hz > 0.0);
  assert(descriptor.audio_high_hz > descriptor.audio_low_hz);
  assert(descriptor.frame_duration_s > 0.0);
  assert(descriptor.cp_duration_s > 0.0);
  assert(descriptor.raw_bitrate_bps > 0);
  assert(descriptor.voice_bitrate_bps > 0);
  assert(descriptor.carrier_count > 0);
  assert(descriptor.codec_id == "codec2");
  assert(contains(descriptor.fec_id, "ldpc"));
  assert(contains(descriptor.modem_id, "ofdm"));
  assert(descriptor.official_baseline);
  assert(!descriptor.emulator);
  assert(descriptor.capabilities.audio_input);
  assert(descriptor.capabilities.audio_output);
  assert(descriptor.capabilities.complex_input);
  assert(descriptor.capabilities.complex_output);
  assert(descriptor.capabilities.bit_payload);
  assert(f700f::freedv_official_codec2_mode_id(descriptor.mode_id) ==
         codec2_mode_id);
}

void official_descriptors_are_representable_test() {
  assert_official_descriptor_shape(f700f::freedv700d_official_descriptor(),
                                   "freedv700d_official", 7);
  assert_official_descriptor_shape(f700f::freedv700e_official_descriptor(),
                                   "freedv700e_official", 13);
}

void official_and_emulator_flags_are_distinct_test() {
  const auto &official_700d = f700f::freedv700d_official_descriptor();
  const auto &emulated_700d = f700f::freedv700d_emulated_descriptor();
  const auto &official_700e = f700f::freedv700e_official_descriptor();
  const auto &emulated_700e = f700f::freedv700e_emulated_descriptor();

  assert(official_700d.mode_id != emulated_700d.mode_id);
  assert(official_700e.mode_id != emulated_700e.mode_id);
  assert(official_700d.official_baseline);
  assert(official_700e.official_baseline);
  assert(!official_700d.emulator);
  assert(!official_700e.emulator);
  assert(emulated_700d.emulator);
  assert(emulated_700e.emulator);
  assert(!emulated_700d.official_baseline);
  assert(!emulated_700e.official_baseline);
}

void registry_exposes_unavailable_status_when_codec2_is_disabled_test() {
  f700f::ModeRegistry registry;
  f700f::register_freedv_official_modes(registry);

  const auto *descriptor = registry.find("freedv700d_official");
  assert(descriptor != nullptr);

  if (!f700f::freedv_official_codec2_available()) {
    assert(contains(descriptor->implementation_status, "unavailable"));
    assert(contains(descriptor->implementation_status, "Codec2"));
    assert(contains(descriptor->implementation_status, "F700F_ENABLE_CODEC2=OFF"));
  }
}

void runtime_reports_skip_or_unavailable_reason_test() {
  f700f::ModeRegistry registry;
  f700f::register_freedv_official_modes(registry);

  auto mode = registry.create("freedv700e_official");
  assert(mode != nullptr);

  if (!f700f::freedv_official_codec2_available()) {
    assert(!mode->configure({.sample_rate_hz = mode->descriptor().sample_rate_hz}));

    const auto encoded = mode->encode({});
    assert(!encoded.ok);
    assert(contains(encoded.error, "freedv700e_official"));
    assert(contains(encoded.error, "unavailable"));
    assert(contains(encoded.error, "F700F_ENABLE_CODEC2=OFF"));

    const auto decoded = mode->decode({});
    assert(!decoded.ok);
    assert(contains(decoded.error, "freedv700e_official"));
    assert(contains(decoded.error, "unavailable"));
    assert(contains(decoded.error, "F700F_ENABLE_CODEC2=OFF"));
  }
}

void codec2_enabled_smoke_test_is_guarded_test() {
  if (f700f::freedv_official_codec2_available()) {
    assert(f700f::freedv_official_codec2_mode_id("freedv700d_official") == 7);
    assert(f700f::freedv_official_codec2_mode_id("freedv700e_official") == 13);
  }
}

} // namespace

int main() {
  official_descriptors_are_representable_test();
  official_and_emulator_flags_are_distinct_test();
  registry_exposes_unavailable_status_when_codec2_is_disabled_test();
  runtime_reports_skip_or_unavailable_reason_test();
  codec2_enabled_smoke_test_is_guarded_test();
  return 0;
}

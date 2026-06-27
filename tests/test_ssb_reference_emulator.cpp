#include <f700f/reference_baselines/freedv_emulator.hpp>
#include <f700f/types.hpp>

#include <cassert>
#include <string>

namespace {

void ssb_standard_3k_descriptor_retrieval_test() {
  const auto &descriptor = f700f::ssb_standard_3k_descriptor();
  assert(descriptor.mode_id == "ssb_standard_3k");
  assert(descriptor.display_name.find("SSB standard 3 kHz reference") != std::string::npos);
  assert(descriptor.rf_bandwidth_hz > 0.0);
  assert(descriptor.audio_bandwidth_hz > 0.0);
  assert(descriptor.audio_bandwidth_hz <= descriptor.rf_bandwidth_hz);
  assert(descriptor.nominal_latency_s >= 0.0);
  assert(descriptor.capabilities.audio_input);
  assert(descriptor.capabilities.audio_output);
  assert(descriptor.capabilities.complex_input);
  assert(descriptor.capabilities.complex_output);
  assert(!descriptor.capabilities.bit_payload);
  assert(!descriptor.emulator || descriptor.official_baseline == false);
  assert(descriptor.codec_id == "analog-voice");
  assert(descriptor.fec_id == "none");
  assert(descriptor.raw_bitrate_bps == 0);
  assert(descriptor.voice_bitrate_bps == 0);
}

void ssb_narrow_1k9_descriptor_retrieval_test() {
  const auto &descriptor = f700f::ssb_narrow_1k9_descriptor();
  assert(descriptor.mode_id == "ssb_narrow_1k9");
  assert(descriptor.display_name.find("SSB narrow 1.9 kHz reference") != std::string::npos);
  assert(descriptor.audio_high_hz > descriptor.audio_low_hz);
  assert(descriptor.capabilities.bit_payload == false);
  assert(!descriptor.emulator || descriptor.official_baseline == false);
  assert(descriptor.raw_bitrate_bps == 0);
}

void ssb_occupied_bandwidths_differ() {
  assert(f700f::ssb_standard_3k_descriptor().audio_bandwidth_hz !=
         f700f::ssb_narrow_1k9_descriptor().audio_bandwidth_hz);
  assert(f700f::ssb_standard_3k_descriptor().rf_bandwidth_hz !=
         f700f::ssb_narrow_1k9_descriptor().rf_bandwidth_hz);
}

void ssb_modes_are_ber_fer_less() {
  const auto &standard = f700f::ssb_standard_3k_descriptor();
  const auto &narrow = f700f::ssb_narrow_1k9_descriptor();
  assert(standard.raw_bitrate_bps == 0);
  assert(standard.voice_bitrate_bps == 0);
  assert(narrow.raw_bitrate_bps == 0);
  assert(narrow.voice_bitrate_bps == 0);
  assert(!standard.capabilities.bit_payload);
  assert(!narrow.capabilities.bit_payload);
}

void mode_registry_selects_ssb_modes() {
  f700f::ModeRegistry registry;
  f700f::register_ssb_reference_modes(registry);
  const auto *standard = registry.find("ssb_standard_3k");
  const auto *narrow = registry.find("ssb_narrow_1k9");
  assert(standard != nullptr);
  assert(narrow != nullptr);
  assert(standard->emulator);
  assert(narrow->emulator);
  assert(!standard->official_baseline);
  assert(!narrow->official_baseline);
}

void ssb_reference_modes_are_registry_visible() {
  f700f::ModeRegistry registry;
  f700f::register_freedv_emulator_modes(registry);
  f700f::register_ssb_reference_modes(registry);
  assert(registry.find("freedv700d_emulated") != nullptr);
  assert(registry.find("ssb_standard_3k") != nullptr);
  assert(registry.find("ssb_narrow_1k9") != nullptr);
}

void ssb_zero_length_audio_does_not_crash() {
  f700f::ModeRegistry registry;
  f700f::register_ssb_reference_modes(registry);
  auto mode = registry.create("ssb_standard_3k");
  assert(mode != nullptr);
  assert(mode->configure({.sample_rate_hz = 8000}));

  f700f::AudioBlock input;
  input.sample_rate_hz = 8000;
  input.mono = {};
  const auto encoded = mode->encode(input);
  assert(encoded.ok);
  assert(encoded.symbols.iq.empty());

  f700f::DecodeResult decoded = mode->decode(encoded.symbols);
  assert(decoded.ok);
  assert(decoded.audio.mono.empty());
  assert(!decoded.error.size());
}

} // namespace

int main() {
  ssb_standard_3k_descriptor_retrieval_test();
  ssb_narrow_1k9_descriptor_retrieval_test();
  ssb_occupied_bandwidths_differ();
  ssb_modes_are_ber_fer_less();
  mode_registry_selects_ssb_modes();
  ssb_reference_modes_are_registry_visible();
  ssb_zero_length_audio_does_not_crash();
  return 0;
}

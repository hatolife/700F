#include <f700f/reference_baselines/freedv_emulator.hpp>
#include <f700f/types.hpp>

#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace {

bool contains(const std::string &haystack, const std::string &needle) {
  return haystack.find(needle) != std::string::npos;
}

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
  assert(descriptor.implementation_status == "emulated_surrogate");
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

void emulator_metadata_is_explicit_test() {
  assert(std::string(f700f::freedv_emulator_model_name()) ==
         "f700f-minimal-freedv700d700e-surrogate");
  assert(!std::string(f700f::freedv_emulator_model_version()).empty());
  assert(contains(f700f::freedv_emulator_limitations(), "not official FreeDV"));
  assert(contains(f700f::freedv_emulator_limitations(), "performance_valid=false"));
}

f700f::AudioBlock make_audio() {
  f700f::AudioBlock audio;
  audio.sample_rate_hz = 8000;
  audio.start_time_s = 0.25;
  audio.mono = {0.0F, 0.25F, -0.25F, 0.5F, -0.5F, 0.125F};
  return audio;
}

bool same_iq(const f700f::ComplexBlock &lhs, const f700f::ComplexBlock &rhs) {
  if (lhs.sample_rate_hz != rhs.sample_rate_hz ||
      lhs.center_frequency_hz != rhs.center_frequency_hz ||
      lhs.start_time_s != rhs.start_time_s || lhs.iq.size() != rhs.iq.size()) {
    return false;
  }
  for (std::size_t i = 0; i < lhs.iq.size(); ++i) {
    if (lhs.iq[i].re != rhs.iq[i].re || lhs.iq[i].im != rhs.iq[i].im) {
      return false;
    }
  }
  return true;
}

void encode_decode_are_minimal_deterministic_surrogates_test(
    const std::string &mode_id) {
  f700f::ModeRegistry registry;
  f700f::register_freedv_emulator_modes(registry);
  auto mode = registry.create(mode_id);
  assert(mode != nullptr);
  assert(mode->configure({.sample_rate_hz = mode->descriptor().sample_rate_hz}));

  const auto audio = make_audio();
  const auto encoded = mode->encode(audio);
  assert(encoded.ok);
  assert(encoded.error.empty());
  assert(encoded.symbols.sample_rate_hz == audio.sample_rate_hz);
  assert(encoded.symbols.start_time_s == audio.start_time_s);
  assert(encoded.symbols.iq.size() == audio.mono.size());
  assert(!encoded.bits.bits.empty());

  const auto decoded = mode->decode(encoded.symbols);
  assert(decoded.ok);
  assert(decoded.error.empty());
  assert(decoded.audio.sample_rate_hz == audio.sample_rate_hz);
  assert(decoded.audio.start_time_s == audio.start_time_s);
  assert(decoded.audio.mono.size() == audio.mono.size());

  mode->reset();
  assert(mode->configure({.sample_rate_hz = mode->descriptor().sample_rate_hz}));
  const auto encoded_again = mode->encode(audio);
  const auto decoded_again = mode->decode(encoded_again.symbols);
  assert(encoded_again.ok);
  assert(decoded_again.ok);
  assert(same_iq(encoded_again.symbols, encoded.symbols));
  assert(encoded_again.bits.bits == encoded.bits.bits);
  assert(decoded_again.audio.mono == decoded.audio.mono);
}

void encode_decode_are_minimal_deterministic_surrogates_test() {
  encode_decode_are_minimal_deterministic_surrogates_test("freedv700d_emulated");
  encode_decode_are_minimal_deterministic_surrogates_test("freedv700e_emulated");
}

} // namespace

int main() {
  descriptors_are_obtainable_test();
  registry_selects_both_modes_test();
  emulator_metadata_is_explicit_test();
  encode_decode_are_minimal_deterministic_surrogates_test();
  return 0;
}

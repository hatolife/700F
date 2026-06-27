#include <f700f/types.hpp>

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

class DummyMode final : public f700f::IMode {
public:
  explicit DummyMode(f700f::ModeDescriptor descriptor)
      : descriptor_(std::move(descriptor)) {}

  const f700f::ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  bool configure(const f700f::ModeRuntimeConfig &config) override {
    if (config.sample_rate_hz != descriptor_.sample_rate_hz) {
      configured_ = false;
      return false;
    }
    configured_ = true;
    return true;
  }

  f700f::EncodeResult encode(const f700f::AudioBlock &audio) override {
    f700f::EncodeResult result;
    result.ok = configured_;
    result.status.frame_index = frame_index_++;
    result.symbols.sample_rate_hz = descriptor_.sample_rate_hz;
    result.symbols.iq.reserve(audio.mono.size());
    for (const float sample : audio.mono) {
      result.symbols.iq.push_back({sample, -sample});
    }
    result.bits.bits.assign(audio.mono.size(), configured_ ? 1 : 0);
    return result;
  }

  f700f::DecodeResult decode(const f700f::ComplexBlock &symbols) override {
    f700f::DecodeResult result;
    result.ok = configured_;
    result.status.frame_index = frame_index_++;
    result.audio.sample_rate_hz = descriptor_.sample_rate_hz;
    result.audio.mono.reserve(symbols.iq.size());
    for (const auto &sample : symbols.iq) {
      result.audio.mono.push_back(sample.re);
    }
    return result;
  }

  void reset() noexcept override { frame_index_ = 0; }

private:
  f700f::ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
};

class DummyFactory final : public f700f::IModeFactory {
public:
  explicit DummyFactory(f700f::ModeDescriptor descriptor)
      : descriptor_(std::move(descriptor)) {}

  const f700f::ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  std::unique_ptr<f700f::IMode> create() const override {
    return std::make_unique<DummyMode>(descriptor_);
  }

private:
  f700f::ModeDescriptor descriptor_;
};

f700f::ModeDescriptor make_dummy_descriptor() {
  f700f::ModeDescriptor descriptor;
  descriptor.mode_id = f700f::ModeId{"dummy.voice"};
  descriptor.display_name = "Dummy voice mode";
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 1600.0;
  descriptor.nominal_latency_s = 0.02;
  descriptor.codec_id = "dummy-codec";
  descriptor.fec_id = "none";
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.complex_output = true;
  return descriptor;
}

void dummy_mode_descriptor_retrieval_test() {
  f700f::ModeRegistry registry;
  auto descriptor = make_dummy_descriptor();
  assert(registry.register_factory(std::make_shared<DummyFactory>(descriptor)));

  const auto *registered = registry.find(f700f::ModeId{"dummy.voice"});
  assert(registered != nullptr);
  assert(registered->mode_id == "dummy.voice");
  assert(registered->display_name == "Dummy voice mode");
  assert(registered->sample_rate_hz == 8000);
}

void dummy_mode_encode_decode_test() {
  auto mode = DummyFactory(make_dummy_descriptor()).create();
  assert(mode->configure({.sample_rate_hz = 8000}));

  f700f::AudioBlock audio;
  audio.sample_rate_hz = 8000;
  audio.mono = {0.25F, -0.5F, 0.75F};
  const auto encoded = mode->encode(audio);
  assert(encoded.ok);
  assert(encoded.symbols.iq.size() == 3);
  assert(encoded.bits.bits == std::vector<std::uint8_t>({1, 1, 1}));

  const auto decoded = mode->decode(encoded.symbols);
  assert(decoded.ok);
  assert(decoded.audio.mono == audio.mono);
}

void reset_test() {
  auto mode = DummyFactory(make_dummy_descriptor()).create();
  assert(mode->configure({.sample_rate_hz = 8000}));
  const auto first = mode->encode({});
  const auto second = mode->encode({});
  assert(first.status.frame_index == 0);
  assert(second.status.frame_index == 1);
  mode->reset();
  const auto after_reset = mode->encode({});
  assert(after_reset.status.frame_index == 0);
}

void invalid_config_test() {
  auto mode = DummyFactory(make_dummy_descriptor()).create();
  assert(!mode->configure({.sample_rate_hz = 48000}));
  const auto encoded = mode->encode({});
  assert(!encoded.ok);
}

void zero_length_block_test() {
  auto mode = DummyFactory(make_dummy_descriptor()).create();
  assert(mode->configure({.sample_rate_hz = 8000}));
  const auto encoded = mode->encode({});
  assert(encoded.ok);
  assert(encoded.symbols.iq.empty());
  const auto decoded = mode->decode(encoded.symbols);
  assert(decoded.ok);
  assert(decoded.audio.mono.empty());
}

void ssb_like_descriptor_representation_test() {
  f700f::ModeDescriptor descriptor;
  descriptor.mode_id = f700f::ModeId{"ssb.reference"};
  descriptor.display_name = "SSB reference";
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 2700.0;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3000.0;
  descriptor.nominal_latency_s = 0.0;
  descriptor.codec_id = "analog-voice";
  descriptor.fec_id = "none";
  descriptor.modem_id = "ssb";
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.complex_output = true;

  assert(descriptor.mode_id == "ssb.reference");
  assert(descriptor.rf_bandwidth_hz == 2700.0);
  assert(descriptor.codec_id == "analog-voice");
  assert(descriptor.fec_id == "none");
}

void freedv_like_descriptor_representation_test() {
  f700f::ModeDescriptor descriptor;
  descriptor.mode_id = f700f::ModeId{"freedv.700d"};
  descriptor.display_name = "FreeDV 700D-like";
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 1500.0;
  descriptor.nominal_latency_s = 0.16;
  descriptor.frame_duration_s = 0.04;
  descriptor.raw_bitrate_bps = 700;
  descriptor.voice_bitrate_bps = 700;
  descriptor.codec_id = "codec2";
  descriptor.fec_id = "freedv";
  descriptor.modem_id = "ofdm";
  descriptor.capabilities.bit_payload = true;
  descriptor.capabilities.complex_input = true;
  descriptor.capabilities.complex_output = true;

  assert(descriptor.mode_id == "freedv.700d");
  assert(descriptor.sample_rate_hz == 8000);
  assert(descriptor.codec_id == "codec2");
  assert(descriptor.fec_id == "freedv");
  assert(descriptor.nominal_latency_s > 0.0);
}

} // namespace

int main() {
  dummy_mode_descriptor_retrieval_test();
  dummy_mode_encode_decode_test();
  reset_test();
  invalid_config_test();
  zero_length_block_test();
  ssb_like_descriptor_representation_test();
  freedv_like_descriptor_representation_test();
  return 0;
}

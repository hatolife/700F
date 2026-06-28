#include <f700f/reference_baselines/freedv_emulator.hpp>

#include <cmath>
#include <memory>
#include <string>
#include <utility>

namespace f700f {
namespace {

constexpr double kPi = 3.14159265358979323846;

ModeDescriptor make_freedv700d_descriptor() {
  ModeDescriptor descriptor;
  descriptor.mode_id = "freedv700d_emulated";
  descriptor.display_name = "FreeDV 700D parameterized emulator";
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 1500.0;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3000.0;
  descriptor.audio_bandwidth_hz = 2700.0;
  descriptor.nominal_latency_s = 0.16;
  descriptor.frame_duration_s = 0.04;
  descriptor.cp_duration_s = 0.0025;
  descriptor.raw_bitrate_bps = 700;
  descriptor.voice_bitrate_bps = 700;
  descriptor.carrier_count = 14;
  descriptor.codec_id = "codec2-placeholder";
  descriptor.fec_id = "parameterized-freedv-fec-placeholder";
  descriptor.modem_id = "parameterized-ofdm-placeholder";
  descriptor.modulation_family = "dqpsk-ofdm-placeholder";
  descriptor.pilot_strategy = "scattered-pilot-placeholder";
  descriptor.official_baseline = false;
  descriptor.emulator = true;
  descriptor.implementation_status =
      "ISSUE-0013 descriptor-only emulator skeleton; encode/decode not implemented";
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_input = true;
  descriptor.capabilities.complex_output = true;
  descriptor.capabilities.bit_payload = true;
  return descriptor;
}

ModeDescriptor make_freedv700e_descriptor() {
  ModeDescriptor descriptor = make_freedv700d_descriptor();
  descriptor.mode_id = "freedv700e_emulated";
  descriptor.display_name = "FreeDV 700E parameterized emulator";
  descriptor.rf_bandwidth_hz = 1600.0;
  descriptor.nominal_latency_s = 0.18;
  descriptor.frame_duration_s = 0.04;
  descriptor.cp_duration_s = 0.0025;
  descriptor.raw_bitrate_bps = 700;
  descriptor.carrier_count = 17;
  descriptor.pilot_strategy = "700e-style scattered-pilot-placeholder";
  descriptor.implementation_status =
      "ISSUE-0013 descriptor-only 700E emulator skeleton; encode/decode not implemented";
  return descriptor;
}

ModeDescriptor make_ssb_standard_3k_descriptor() {
  ModeDescriptor descriptor;
  descriptor.mode_id = "ssb_standard_3k";
  descriptor.display_name = "SSB standard 3 kHz reference";
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 3300.0;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3300.0;
  descriptor.audio_bandwidth_hz = 3000.0;
  descriptor.nominal_latency_s = 0.02;
  descriptor.frame_duration_s = 0.04;
  descriptor.cp_duration_s = 0.0;
  descriptor.raw_bitrate_bps = 0;
  descriptor.voice_bitrate_bps = 0;
  descriptor.carrier_count = 1;
  descriptor.codec_id = "analog-voice";
  descriptor.fec_id = "none";
  descriptor.modem_id = "ssb";
  descriptor.modulation_family = "analog-ssb";
  descriptor.pilot_strategy = "none";
  descriptor.official_baseline = false;
  descriptor.emulator = true;
  descriptor.implementation_status =
      "ISSUE-0011 reference-mode placeholder; no high-fidelity SSB RF emulation";
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_input = true;
  descriptor.capabilities.complex_output = true;
  descriptor.capabilities.bit_payload = false;
  return descriptor;
}

ModeDescriptor make_ssb_narrow_1k9_descriptor() {
  ModeDescriptor descriptor = make_ssb_standard_3k_descriptor();
  descriptor.mode_id = "ssb_narrow_1k9";
  descriptor.display_name = "SSB narrow 1.9 kHz reference";
  descriptor.rf_bandwidth_hz = 1900.0;
  descriptor.audio_high_hz = 1900.0;
  descriptor.audio_bandwidth_hz = 1600.0;
  descriptor.nominal_latency_s = 0.03;
  descriptor.implementation_status =
      "ISSUE-0011 reference-mode placeholder; no high-fidelity SSB RF emulation";
  return descriptor;
}

const ModeDescriptor &descriptor_for_id(const ModeId &mode_id) noexcept;

float apply_simple_lowpass(float input, float &history, double cutoff_hz, std::uint32_t sample_rate_hz) {
  if (sample_rate_hz == 0 || cutoff_hz <= 0.0) {
    return input;
  }
  const double dt = 1.0 / static_cast<double>(sample_rate_hz);
  const double tau = 1.0 / (2.0 * kPi * cutoff_hz);
  const double alpha = dt / (tau + dt);
  history += static_cast<float>(alpha * (static_cast<double>(input) - static_cast<double>(history)));
  return history;
}

class FreedvEmulatorMode final : public IMode {
public:
  explicit FreedvEmulatorMode(ModeId mode_id)
      : descriptor_(descriptor_for_id(mode_id)), is_freedv_mode_(mode_id.find("freedv") == 0) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  bool configure(const ModeRuntimeConfig &config) override {
    configured_ = config.sample_rate_hz == 0 || config.sample_rate_hz == descriptor_.sample_rate_hz;
    return configured_;
  }

  EncodeResult encode(const AudioBlock &audio) override {
    EncodeResult result;
    result.status.frame_index = frame_index_++;
    if (!configured_) {
      result.error = descriptor_.mode_id + " not configured";
      return result;
    }
    if (is_freedv_mode_) {
      result.error = descriptor_.mode_id +
                     " ISSUE-0013 encode not implemented in emulator skeleton";
      return result;
    }
    result.ok = true;
    result.status.sync = true;
    result.status.fec_ok = true;
    result.status.confidence = 1.0F;
    result.symbols.sample_rate_hz = descriptor_.sample_rate_hz;
    result.symbols.center_frequency_hz = 0.0;
    result.symbols.start_time_s = audio.start_time_s;
    result.symbols.iq.reserve(audio.mono.size());
    for (const float sample : audio.mono) {
      const float limited =
          apply_simple_lowpass(sample, encode_filter_state_, descriptor_.audio_bandwidth_hz / 2.0, descriptor_.sample_rate_hz);
      result.symbols.iq.push_back({limited, 0.0F});
    }
    return result;
  }

  DecodeResult decode(const ComplexBlock &symbols) override {
    DecodeResult result;
    result.status.frame_index = frame_index_++;
    result.audio.sample_rate_hz = descriptor_.sample_rate_hz;
    result.audio.start_time_s = symbols.start_time_s;
    if (!configured_) {
      result.error = descriptor_.mode_id + " not configured";
      return result;
    }
    if (is_freedv_mode_) {
      result.error = descriptor_.mode_id +
                     " ISSUE-0013 decode not implemented in emulator skeleton";
      return result;
    }
    result.ok = true;
    result.status.sync = true;
    result.status.fec_ok = true;
    result.status.confidence = 1.0F;
    result.audio.mono.reserve(symbols.iq.size());
    for (const auto &sample : symbols.iq) {
      const float limited = apply_simple_lowpass(sample.re, decode_filter_state_, descriptor_.audio_bandwidth_hz / 2.0,
                                               descriptor_.sample_rate_hz);
      result.audio.mono.push_back(limited);
    }
    return result;
  }

  void reset() noexcept override {
    frame_index_ = 0;
    encode_filter_state_ = 0.0F;
    decode_filter_state_ = 0.0F;
  }

private:
  ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
  float encode_filter_state_ = 0.0F;
  float decode_filter_state_ = 0.0F;
  const bool is_freedv_mode_ = false;
};

class FreedvEmulatorFactory final : public IModeFactory {
public:
  explicit FreedvEmulatorFactory(ModeId mode_id)
      : descriptor_(descriptor_for_id(mode_id)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  std::unique_ptr<IMode> create() const override {
    return std::make_unique<FreedvEmulatorMode>(descriptor_.mode_id);
  }

private:
  ModeDescriptor descriptor_;
};

const ModeDescriptor &descriptor_for_id(const ModeId &mode_id) noexcept {
  static const ModeDescriptor descriptor_700d = make_freedv700d_descriptor();
  static const ModeDescriptor descriptor_700e = make_freedv700e_descriptor();
  static const ModeDescriptor descriptor_ssb_standard_3k = make_ssb_standard_3k_descriptor();
  static const ModeDescriptor descriptor_ssb_narrow_1k9 = make_ssb_narrow_1k9_descriptor();
  if (mode_id == "freedv700e_emulated") {
    return descriptor_700e;
  }
  if (mode_id == "ssb_narrow_1k9") {
    return descriptor_ssb_narrow_1k9;
  }
  if (mode_id == "ssb_standard_3k") {
    return descriptor_ssb_standard_3k;
  }
  return descriptor_700d;
}

} // namespace

const ModeDescriptor &freedv700d_emulated_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_freedv700d_descriptor();
  return descriptor;
}

const ModeDescriptor &freedv700e_emulated_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_freedv700e_descriptor();
  return descriptor;
}

const ModeDescriptor &ssb_standard_3k_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_ssb_standard_3k_descriptor();
  return descriptor;
}

const ModeDescriptor &ssb_narrow_1k9_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_ssb_narrow_1k9_descriptor();
  return descriptor;
}

void register_freedv_emulator_modes(ModeRegistry &registry) {
  registry.register_factory(
      std::make_shared<FreedvEmulatorFactory>("freedv700d_emulated"));
  registry.register_factory(
      std::make_shared<FreedvEmulatorFactory>("freedv700e_emulated"));
}

void register_ssb_reference_modes(ModeRegistry &registry) {
  registry.register_factory(std::make_shared<FreedvEmulatorFactory>("ssb_standard_3k"));
  registry.register_factory(std::make_shared<FreedvEmulatorFactory>("ssb_narrow_1k9"));
}

std::shared_ptr<IModeFactory> make_freedv700d_emulated_mode_factory() {
  return std::make_shared<FreedvEmulatorFactory>("freedv700d_emulated");
}

std::shared_ptr<IModeFactory> make_freedv700e_emulated_mode_factory() {
  return std::make_shared<FreedvEmulatorFactory>("freedv700e_emulated");
}

std::shared_ptr<IModeFactory> make_ssb_standard_3k_mode_factory() {
  return std::make_shared<FreedvEmulatorFactory>("ssb_standard_3k");
}

std::shared_ptr<IModeFactory> make_ssb_narrow_1k9_mode_factory() {
  return std::make_shared<FreedvEmulatorFactory>("ssb_narrow_1k9");
}

} // namespace f700f

#include <f700f/reference_baselines/freedv_emulator.hpp>

#include <memory>
#include <string>
#include <utility>

namespace f700f {
namespace {

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

const ModeDescriptor &descriptor_for_id(const ModeId &mode_id) noexcept {
  static const ModeDescriptor descriptor_700d = make_freedv700d_descriptor();
  static const ModeDescriptor descriptor_700e = make_freedv700e_descriptor();
  return mode_id == "freedv700e_emulated" ? descriptor_700e : descriptor_700d;
}

class FreedvEmulatorMode final : public IMode {
public:
  explicit FreedvEmulatorMode(ModeId mode_id)
      : descriptor_(descriptor_for_id(mode_id)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  bool configure(const ModeRuntimeConfig &config) override {
    configured_ = config.sample_rate_hz == 0 ||
                  config.sample_rate_hz == descriptor_.sample_rate_hz;
    return configured_;
  }

  EncodeResult encode(const AudioBlock &) override {
    EncodeResult result;
    result.ok = false;
    result.status.frame_index = frame_index_++;
    result.error = configured_
                       ? descriptor_.mode_id +
                             " ISSUE-0013 encode not implemented in emulator skeleton"
                       : descriptor_.mode_id + " not configured";
    return result;
  }

  DecodeResult decode(const ComplexBlock &) override {
    DecodeResult result;
    result.ok = false;
    result.status.frame_index = frame_index_++;
    result.error = configured_
                       ? descriptor_.mode_id +
                             " ISSUE-0013 decode not implemented in emulator skeleton"
                       : descriptor_.mode_id + " not configured";
    return result;
  }

  void reset() noexcept override { frame_index_ = 0; }

private:
  ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
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

} // namespace

const ModeDescriptor &freedv700d_emulated_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_freedv700d_descriptor();
  return descriptor;
}

const ModeDescriptor &freedv700e_emulated_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_freedv700e_descriptor();
  return descriptor;
}

void register_freedv_emulator_modes(ModeRegistry &registry) {
  registry.register_factory(
      std::make_shared<FreedvEmulatorFactory>("freedv700d_emulated"));
  registry.register_factory(
      std::make_shared<FreedvEmulatorFactory>("freedv700e_emulated"));
}

} // namespace f700f

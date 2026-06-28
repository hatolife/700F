#include <f700f/candidate_profiles.hpp>

#include <memory>
#include <string>
#include <utility>

namespace f700f {
namespace {

ModeDescriptor make_common_descriptor() {
  ModeDescriptor descriptor;
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 1900.0;
  descriptor.audio_low_hz = 300.0;
  descriptor.nominal_latency_s = 0.18;
  descriptor.frame_duration_s = 0.04;
  descriptor.cp_duration_s = 0.0025;
  descriptor.raw_bitrate_bps = 700;
  descriptor.voice_bitrate_bps = 700;
  descriptor.text_bitrate_bps = 0;
  descriptor.carrier_count = 19;
  descriptor.codec_id = "codec2-700f-candidate-placeholder";
  descriptor.modem_id = "ofdm-700f-candidate-placeholder";
  descriptor.modulation_family = "qpsk-ofdm-candidate";
  descriptor.pilot_strategy = "candidate-scattered-and-continual-pilots";
  descriptor.official_baseline = false;
  descriptor.emulator = false;
  descriptor.implementation_status = "profile_only";
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_input = true;
  descriptor.capabilities.complex_output = true;
  descriptor.capabilities.bit_payload = true;
  descriptor.capabilities.soft_bits = true;
  return descriptor;
}

ModeDescriptor make_balanced_descriptor() {
  ModeDescriptor descriptor = make_common_descriptor();
  descriptor.mode_id = "freedv700f_a_balanced";
  descriptor.display_name = "FreeDV 700F candidate A balanced profile";
  descriptor.audio_high_hz = 3300.0;
  descriptor.audio_bandwidth_hz = 3000.0;
  descriptor.fec_id = "700f-candidate-medium-fec-policy";
  descriptor.pilot_strategy = "medium-density candidate pilots";
  return descriptor;
}

ModeDescriptor make_robust_descriptor() {
  ModeDescriptor descriptor = make_common_descriptor();
  descriptor.mode_id = "freedv700f_b_robust";
  descriptor.display_name = "FreeDV 700F candidate B robust profile";
  descriptor.audio_high_hz = 3150.0;
  descriptor.audio_bandwidth_hz = 2850.0;
  descriptor.nominal_latency_s = 0.24;
  descriptor.carrier_count = 21;
  descriptor.fec_id = "700f-candidate-strong-fec-policy";
  descriptor.pilot_strategy = "high-density candidate pilots";
  return descriptor;
}

ModeDescriptor make_quality_descriptor() {
  ModeDescriptor descriptor = make_common_descriptor();
  descriptor.mode_id = "freedv700f_c_quality";
  descriptor.display_name = "FreeDV 700F candidate C quality profile";
  descriptor.sample_rate_hz = 12000;
  descriptor.audio_high_hz = 4000.0;
  descriptor.audio_bandwidth_hz = 3700.0;
  descriptor.nominal_latency_s = 0.16;
  descriptor.carrier_count = 17;
  descriptor.fec_id = "700f-candidate-light-fec-policy";
  descriptor.pilot_strategy = "lower-overhead candidate pilots";
  return descriptor;
}

const ModeDescriptor *descriptor_for_id(const ModeId &mode_id) noexcept {
  if (mode_id == "freedv700f_a_balanced") {
    return &freedv700f_a_balanced_descriptor();
  }
  if (mode_id == "freedv700f_b_robust") {
    return &freedv700f_b_robust_descriptor();
  }
  if (mode_id == "freedv700f_c_quality") {
    return &freedv700f_c_quality_descriptor();
  }
  return nullptr;
}

class CandidateProfileMode final : public IMode {
public:
  explicit CandidateProfileMode(ModeDescriptor descriptor)
      : descriptor_(std::move(descriptor)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  bool configure(const ModeRuntimeConfig &config) override {
    configured_ =
        config.sample_rate_hz == 0 || config.sample_rate_hz == descriptor_.sample_rate_hz;
    return configured_;
  }

  EncodeResult encode(const AudioBlock &) override {
    EncodeResult result;
    result.status.frame_index = frame_index_++;
    if (!configured_) {
      result.error = descriptor_.mode_id + " not configured";
      return result;
    }
    result.error = descriptor_.mode_id +
                   " ISSUE-0018 profile_only encode not implemented";
    return result;
  }

  DecodeResult decode(const ComplexBlock &) override {
    DecodeResult result;
    result.status.frame_index = frame_index_++;
    result.audio.sample_rate_hz = descriptor_.sample_rate_hz;
    if (!configured_) {
      result.error = descriptor_.mode_id + " not configured";
      return result;
    }
    result.error = descriptor_.mode_id +
                   " ISSUE-0018 profile_only decode not implemented";
    return result;
  }

  void reset() noexcept override {
    frame_index_ = 0;
  }

private:
  ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
};

class CandidateProfileFactory final : public IModeFactory {
public:
  explicit CandidateProfileFactory(ModeDescriptor descriptor)
      : descriptor_(std::move(descriptor)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  std::unique_ptr<IMode> create() const override {
    return std::make_unique<CandidateProfileMode>(descriptor_);
  }

private:
  ModeDescriptor descriptor_;
};

} // namespace

const ModeDescriptor &freedv700f_a_balanced_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_balanced_descriptor();
  return descriptor;
}

const ModeDescriptor &freedv700f_b_robust_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_robust_descriptor();
  return descriptor;
}

const ModeDescriptor &freedv700f_c_quality_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_quality_descriptor();
  return descriptor;
}

std::shared_ptr<IModeFactory> make_700f_candidate_profile_factory(
    ModeId mode_id) {
  const auto *descriptor = descriptor_for_id(mode_id);
  if (descriptor == nullptr) {
    return nullptr;
  }
  return std::make_shared<CandidateProfileFactory>(*descriptor);
}

void register_700f_candidate_profiles(ModeRegistry &registry) {
  registry.register_factory(
      make_700f_candidate_profile_factory("freedv700f_a_balanced"));
  registry.register_factory(
      make_700f_candidate_profile_factory("freedv700f_b_robust"));
  registry.register_factory(
      make_700f_candidate_profile_factory("freedv700f_c_quality"));
}

} // namespace f700f

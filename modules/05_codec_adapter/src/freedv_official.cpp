#include <f700f/codec_adapter/freedv_official.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#if F700F_CODEC2_AVAILABLE
#include <freedv_api.h>
#endif

namespace f700f {
namespace {

constexpr int kFreedv700dCodec2ModeId = 7;
constexpr int kFreedv700eCodec2ModeId = 13;

#if F700F_CODEC2_AVAILABLE
static_assert(FREEDV_MODE_700D == kFreedv700dCodec2ModeId);
static_assert(FREEDV_MODE_700E == kFreedv700eCodec2ModeId);
#endif

bool codec2_available() noexcept {
#if F700F_CODEC2_AVAILABLE
  return true;
#else
  return false;
#endif
}

std::string codec2_unavailable_reason() {
  return "unavailable: Codec2 official adapter skipped because "
         "F700F_ENABLE_CODEC2=OFF";
}

std::string codec2_runtime_status() {
  if (!codec2_available()) {
    return codec2_unavailable_reason();
  }
  return "available: Codec2 FreeDV API headers detected; ISSUE-0012 runtime "
         "waveform binding is a guarded skeleton";
}

ModeDescriptor make_common_descriptor() {
  ModeDescriptor descriptor;
  descriptor.sample_rate_hz = 8000;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3000.0;
  descriptor.audio_bandwidth_hz = 2700.0;
  descriptor.raw_bitrate_bps = 700;
  descriptor.voice_bitrate_bps = 700;
  descriptor.codec_id = "codec2";
  descriptor.modulation_family = "qpsk-ofdm";
  descriptor.official_baseline = true;
  descriptor.emulator = false;
  descriptor.implementation_status = codec2_runtime_status();
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_input = true;
  descriptor.capabilities.complex_output = true;
  descriptor.capabilities.bit_payload = true;
  return descriptor;
}

ModeDescriptor make_freedv700d_descriptor() {
  ModeDescriptor descriptor = make_common_descriptor();
  descriptor.mode_id = "freedv700d_official";
  descriptor.display_name = "FreeDV 700D official Codec2 baseline";
  descriptor.rf_bandwidth_hz = 1500.0;
  descriptor.nominal_latency_s = 0.16;
  descriptor.frame_duration_s = 0.16;
  descriptor.cp_duration_s = 0.002;
  descriptor.carrier_count = 17;
  descriptor.fec_id = "codec2-ldpc-HRA_112_112";
  descriptor.modem_id = "codec2-ofdm-700d";
  descriptor.pilot_strategy = "edge-pilots";
  return descriptor;
}

ModeDescriptor make_freedv700e_descriptor() {
  ModeDescriptor descriptor = make_common_descriptor();
  descriptor.mode_id = "freedv700e_official";
  descriptor.display_name = "FreeDV 700E official Codec2 baseline";
  descriptor.rf_bandwidth_hz = 1600.0;
  descriptor.nominal_latency_s = 0.08;
  descriptor.frame_duration_s = 0.08;
  descriptor.cp_duration_s = 0.006;
  descriptor.carrier_count = 21;
  descriptor.fec_id = "codec2-ldpc-HRA_56_56";
  descriptor.modem_id = "codec2-ofdm-700e";
  descriptor.pilot_strategy = "voice2-unique-word-pilots";
  return descriptor;
}

const ModeDescriptor &descriptor_for_id(const ModeId &mode_id) noexcept {
  static const ModeDescriptor descriptor_700d = make_freedv700d_descriptor();
  static const ModeDescriptor descriptor_700e = make_freedv700e_descriptor();
  return mode_id == "freedv700e_official" ? descriptor_700e : descriptor_700d;
}

class FreedvOfficialMode final : public IMode {
public:
  explicit FreedvOfficialMode(ModeId mode_id)
      : descriptor_(descriptor_for_id(mode_id)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  bool configure(const ModeRuntimeConfig &config) override {
    if (!codec2_available()) {
      configured_ = false;
      last_config_error_ = codec2_unavailable_reason();
      return false;
    }
    if (config.sample_rate_hz != 0 &&
        config.sample_rate_hz != descriptor_.sample_rate_hz) {
      configured_ = false;
      last_config_error_ = descriptor_.mode_id +
                           " unsupported sample rate for official Codec2 mode";
      return false;
    }
    configured_ = true;
    last_config_error_.clear();
    return true;
  }

  EncodeResult encode(const AudioBlock &) override {
    EncodeResult result;
    result.ok = false;
    result.status.frame_index = frame_index_++;
    result.error = runtime_error("encode");
    return result;
  }

  DecodeResult decode(const ComplexBlock &) override {
    DecodeResult result;
    result.ok = false;
    result.status.frame_index = frame_index_++;
    result.error = runtime_error("decode");
    return result;
  }

  void reset() noexcept override { frame_index_ = 0; }

private:
  std::string runtime_error(const std::string &operation) const {
    if (!codec2_available()) {
      return descriptor_.mode_id + " " + codec2_unavailable_reason();
    }
    if (!configured_) {
      if (!last_config_error_.empty()) {
        return descriptor_.mode_id + " not configured: " + last_config_error_;
      }
      return descriptor_.mode_id + " not configured";
    }
    return descriptor_.mode_id + " runtime failure: ISSUE-0012 official " +
           operation + " waveform binding is not implemented yet";
  }

  ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
  std::string last_config_error_;
};

class FreedvOfficialFactory final : public IModeFactory {
public:
  explicit FreedvOfficialFactory(ModeId mode_id)
      : descriptor_(descriptor_for_id(mode_id)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  std::unique_ptr<IMode> create() const override {
    return std::make_unique<FreedvOfficialMode>(descriptor_.mode_id);
  }

private:
  ModeDescriptor descriptor_;
};

} // namespace

const ModeDescriptor &freedv700d_official_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_freedv700d_descriptor();
  return descriptor;
}

const ModeDescriptor &freedv700e_official_descriptor() noexcept {
  static const ModeDescriptor descriptor = make_freedv700e_descriptor();
  return descriptor;
}

bool freedv_official_codec2_available() noexcept {
  return codec2_available();
}

int freedv_official_codec2_mode_id(const ModeId &mode_id) noexcept {
  if (mode_id == "freedv700d_official") {
    return kFreedv700dCodec2ModeId;
  }
  if (mode_id == "freedv700e_official") {
    return kFreedv700eCodec2ModeId;
  }
  return -1;
}

void register_freedv_official_modes(ModeRegistry &registry) {
  registry.register_factory(
      std::make_shared<FreedvOfficialFactory>("freedv700d_official"));
  registry.register_factory(
      std::make_shared<FreedvOfficialFactory>("freedv700e_official"));
}

} // namespace f700f

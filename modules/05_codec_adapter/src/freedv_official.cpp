#include <f700f/codec_adapter/freedv_official.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

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
  return "available: Codec2 FreeDV runtime linked; ISSUE-0036 official "
         "Mode-boundary roundtrip smoke enabled";
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
#if F700F_CODEC2_AVAILABLE
    const auto mode_id = freedv_official_codec2_mode_id(descriptor_.mode_id);
    tx_.reset(freedv_open(mode_id));
    rx_.reset(freedv_open(mode_id));
    if (!tx_ || !rx_) {
      configured_ = false;
      last_config_error_ =
          descriptor_.mode_id + " failed to open Codec2 FreeDV runtime";
      tx_.reset();
      rx_.reset();
      return false;
    }
    speech_sample_rate_hz_ =
        static_cast<SampleRateHz>(freedv_get_speech_sample_rate(tx_.get()));
    modem_sample_rate_hz_ =
        static_cast<SampleRateHz>(freedv_get_modem_sample_rate(tx_.get()));
    n_speech_samples_ = freedv_get_n_speech_samples(tx_.get());
    n_max_speech_samples_ = freedv_get_n_max_speech_samples(rx_.get());
    n_nom_modem_samples_ = freedv_get_n_nom_modem_samples(tx_.get());
    n_max_modem_samples_ = freedv_get_n_max_modem_samples(rx_.get());
    if (speech_sample_rate_hz_ == 0 || modem_sample_rate_hz_ == 0 ||
        n_speech_samples_ <= 0 || n_max_speech_samples_ <= 0 ||
        n_nom_modem_samples_ <= 0 || n_max_modem_samples_ <= 0) {
      configured_ = false;
      last_config_error_ =
          descriptor_.mode_id + " Codec2 FreeDV runtime reported invalid sizes";
      tx_.reset();
      rx_.reset();
      return false;
    }
#endif
    configured_ = true;
    last_config_error_.clear();
    return true;
  }

  EncodeResult encode(const AudioBlock &audio) override {
    EncodeResult result;
    result.status.frame_index = frame_index_++;
    if (!ready_for_runtime(result.error, "encode")) {
      result.ok = false;
      return result;
    }
    if (audio.mono.empty()) {
      result.ok = true;
      result.symbols.sample_rate_hz = modem_sample_rate_hz_;
      return result;
    }
#if F700F_CODEC2_AVAILABLE
    std::vector<short> speech(static_cast<std::size_t>(n_speech_samples_), 0);
    last_input_sample_count_ = audio.mono.size();
    for (std::size_t i = 0; i < speech.size() && i < audio.mono.size(); ++i) {
      speech[i] = float_to_short(audio.mono[i]);
    }
    std::vector<COMP> modem(static_cast<std::size_t>(n_nom_modem_samples_));
    freedv_comptx(tx_.get(), modem.data(), speech.data());
    result.symbols.sample_rate_hz = modem_sample_rate_hz_;
    result.symbols.iq.reserve(modem.size());
    for (const auto &sample : modem) {
      result.symbols.iq.push_back({sample.real, sample.imag});
    }
    result.ok = true;
#else
    result.ok = false;
#endif
    return result;
  }

  DecodeResult decode(const ComplexBlock &symbols) override {
    DecodeResult result;
    result.status.frame_index = frame_index_++;
    result.audio.sample_rate_hz = speech_sample_rate_hz_;
    if (!ready_for_runtime(result.error, "decode")) {
      result.ok = false;
      return result;
    }
    if (symbols.iq.empty()) {
      result.ok = true;
      return result;
    }
#if F700F_CODEC2_AVAILABLE
    std::size_t offset = 0;
    std::vector<short> speech(static_cast<std::size_t>(n_max_speech_samples_));
    while (offset < symbols.iq.size()) {
      const auto nin = freedv_nin(rx_.get());
      if (nin <= 0 || nin > n_max_modem_samples_) {
        result.ok = false;
        result.error = descriptor_.mode_id +
                       " Codec2 FreeDV runtime returned invalid freedv_nin()";
        return result;
      }
      if (offset + static_cast<std::size_t>(nin) > symbols.iq.size()) {
        break;
      }
      std::vector<COMP> modem(static_cast<std::size_t>(nin));
      for (std::size_t i = 0; i < modem.size(); ++i) {
        modem[i].real = symbols.iq[offset + i].re;
        modem[i].imag = symbols.iq[offset + i].im;
      }
      const auto nout = freedv_comprx(rx_.get(), speech.data(), modem.data());
      if (nout < 0 || nout > n_max_speech_samples_) {
        result.ok = false;
        result.error =
            descriptor_.mode_id + " Codec2 FreeDV runtime returned invalid RX output size";
        return result;
      }
      for (int i = 0; i < nout; ++i) {
        result.audio.mono.push_back(short_to_float(speech[static_cast<std::size_t>(i)]));
      }
      offset += static_cast<std::size_t>(nin);
    }
    if (last_input_sample_count_ > 0) {
      result.audio.mono.resize(last_input_sample_count_, 0.0F);
    }
    result.ok = true;
#else
    result.ok = false;
#endif
    return result;
  }

  void reset() noexcept override { frame_index_ = 0; }

private:
  struct FreedvCloser {
#if F700F_CODEC2_AVAILABLE
    void operator()(struct freedv *handle) const noexcept {
      if (handle != nullptr) {
        freedv_close(handle);
      }
    }
#else
    void operator()(void *) const noexcept {}
#endif
  };

#if F700F_CODEC2_AVAILABLE
  using FreedvHandle = std::unique_ptr<struct freedv, FreedvCloser>;
#else
  using FreedvHandle = std::unique_ptr<void, FreedvCloser>;
#endif

  static short float_to_short(float value) noexcept {
    const auto clipped = std::clamp(value, -1.0F, 1.0F);
    return static_cast<short>(std::lrint(clipped * 32767.0F));
  }

  static float short_to_float(short value) noexcept {
    return static_cast<float>(value) / 32768.0F;
  }

  bool ready_for_runtime(std::string &error, const std::string &operation) const {
    if (!codec2_available()) {
      error = descriptor_.mode_id + " " + codec2_unavailable_reason();
      return false;
    }
    if (!configured_) {
      if (!last_config_error_.empty()) {
        error = descriptor_.mode_id + " not configured: " + last_config_error_;
        return false;
      }
      error = descriptor_.mode_id + " not configured";
      return false;
    }
    if (!tx_ || !rx_) {
      error = descriptor_.mode_id + " Codec2 FreeDV runtime is not open for " +
              operation;
      return false;
    }
    return true;
  }

  ModeDescriptor descriptor_;
  FreedvHandle tx_;
  FreedvHandle rx_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
  std::string last_config_error_;
  SampleRateHz speech_sample_rate_hz_ = 8000;
  SampleRateHz modem_sample_rate_hz_ = 8000;
  int n_speech_samples_ = 0;
  int n_max_speech_samples_ = 0;
  int n_nom_modem_samples_ = 0;
  int n_max_modem_samples_ = 0;
  std::size_t last_input_sample_count_ = 0;
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
  registry.register_factory(make_freedv700d_official_mode_factory());
  registry.register_factory(make_freedv700e_official_mode_factory());
}

std::shared_ptr<IModeFactory> make_freedv700d_official_mode_factory() {
  return std::make_shared<FreedvOfficialFactory>("freedv700d_official");
}

std::shared_ptr<IModeFactory> make_freedv700e_official_mode_factory() {
  return std::make_shared<FreedvOfficialFactory>("freedv700e_official");
}

} // namespace f700f

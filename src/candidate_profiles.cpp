#include <f700f/candidate_profiles.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace f700f {
namespace {

constexpr SampleRateHz k700fASampleRateHz = 8000;
constexpr double k700fASymbolRateBaud = 1200.0;
constexpr double k700fAOccupiedBandwidthHz = 1900.0;
constexpr float kQpskScale = 0.70710678118F;

std::vector<std::string> minimal_qpsk_limitations() {
  return {"ISSUE-0042 minimal QPSK-like baseband prototype only; "
          "synthetic codec; no FEC; no final synchronization; not final modem; "
          "not official FreeDV; not valid for real downselect"};
}

ModemFrameDescriptor make_minimal_qpsk_descriptor(
    const ModeId &mode_id, std::uint64_t frame_index, SampleRateHz sample_rate_hz,
    Seed seed) {
  ModemFrameDescriptor descriptor;
  descriptor.mode_id = mode_id;
  descriptor.frame_index = frame_index;
  descriptor.sample_rate = SampleRate{sample_rate_hz};
  descriptor.symbol_rate = SymbolRate{k700fASymbolRateBaud};
  descriptor.seed = seed;
  descriptor.modem_family = "minimal_qpsk";
  descriptor.carrier_layout.family = "minimal_qpsk_single_carrier";
  descriptor.carrier_layout.carrier_count = 1;
  descriptor.carrier_layout.occupied_bandwidth_hz = k700fAOccupiedBandwidthHz;
  descriptor.carrier_layout.limitations = minimal_qpsk_limitations();
  descriptor.sync.family = "none";
  descriptor.sync.locked = false;
  descriptor.sync.limitations = {
      "ISSUE-0042 does not implement final timing or carrier synchronization"};
  descriptor.fec.family = "none";
  descriptor.fec.enabled = false;
  descriptor.fec.limitations = {"ISSUE-0042 does not implement FEC"};
  descriptor.codec.family = "synthetic";
  descriptor.codec.encoded = true;
  descriptor.codec.decoded = true;
  descriptor.codec.limitations = {
      "ISSUE-0042 uses synthetic PCM-byte payloads, not Codec2"};
  descriptor.limitations = minimal_qpsk_limitations();
  return descriptor;
}

std::vector<std::uint8_t> audio_to_payload_bytes(const AudioBlock &audio) {
  std::vector<std::uint8_t> payload;
  payload.reserve(audio.mono.size() * 2);
  for (const auto sample : audio.mono) {
    const auto finite_sample = std::isfinite(sample) ? sample : 0.0F;
    const auto clipped = std::clamp(finite_sample, -1.0F, 1.0F);
    const auto quantized =
        static_cast<std::int16_t>(std::lround(clipped * 32767.0F));
    payload.push_back(static_cast<std::uint8_t>(quantized & 0xFF));
    payload.push_back(static_cast<std::uint8_t>((quantized >> 8) & 0xFF));
  }
  return payload;
}

std::vector<float> payload_bytes_to_audio(
    const std::vector<std::uint8_t> &payload,
    std::uint64_t expected_audio_samples) {
  const auto available_samples = payload.size() / 2;
  const auto sample_count = expected_audio_samples == 0
                                ? available_samples
                                : static_cast<std::size_t>(expected_audio_samples);
  std::vector<float> audio;
  audio.reserve(sample_count);
  for (std::size_t i = 0; i < sample_count; ++i) {
    const auto byte_index = i * 2;
    std::int16_t sample = 0;
    if (byte_index + 1 < payload.size()) {
      sample = static_cast<std::int16_t>(
          static_cast<std::uint16_t>(payload[byte_index]) |
          (static_cast<std::uint16_t>(payload[byte_index + 1]) << 8));
    }
    audio.push_back(static_cast<float>(sample) / 32767.0F);
  }
  return audio;
}

std::vector<std::uint8_t> payload_bytes_to_bits(
    const std::vector<std::uint8_t> &payload) {
  std::vector<std::uint8_t> bits;
  bits.reserve(payload.size() * 8);
  for (const auto byte : payload) {
    for (int bit = 7; bit >= 0; --bit) {
      bits.push_back(static_cast<std::uint8_t>((byte >> bit) & 0x01U));
    }
  }
  return bits;
}

std::vector<std::uint8_t> bits_to_payload_bytes(
    const std::vector<std::uint8_t> &bits) {
  std::vector<std::uint8_t> payload;
  payload.reserve((bits.size() + 7) / 8);
  for (std::size_t offset = 0; offset < bits.size(); offset += 8) {
    std::uint8_t byte = 0;
    for (std::size_t bit = 0; bit < 8 && offset + bit < bits.size(); ++bit) {
      byte = static_cast<std::uint8_t>(
          (byte << 1U) | (bits[offset + bit] == 0 ? 0U : 1U));
    }
    if (bits.size() - offset < 8) {
      byte = static_cast<std::uint8_t>(byte << (8 - (bits.size() - offset)));
    }
    payload.push_back(byte);
  }
  return payload;
}

Complex32 qpsk_symbol(std::uint8_t first_bit, std::uint8_t second_bit) {
  return {first_bit == 0 ? kQpskScale : -kQpskScale,
          second_bit == 0 ? kQpskScale : -kQpskScale};
}

std::vector<std::uint8_t> qpsk_symbols_to_bits(
    const std::vector<Complex32> &symbols) {
  std::vector<std::uint8_t> bits;
  bits.reserve(symbols.size() * 2);
  for (const auto &symbol : symbols) {
    bits.push_back(symbol.re < 0.0F ? 1U : 0U);
    bits.push_back(symbol.im < 0.0F ? 1U : 0U);
  }
  return bits;
}

ModeDescriptor make_common_descriptor() {
  ModeDescriptor descriptor;
  descriptor.sample_rate_hz = k700fASampleRateHz;
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
  descriptor.implementation_status = "surrogate";
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
  descriptor.codec_id = "synthetic-700f-a-prototype";
  descriptor.fec_id = "none";
  descriptor.modem_id = "minimal-qpsk-baseband-700f-a";
  descriptor.modulation_family = "qpsk";
  descriptor.pilot_strategy = "none";
  descriptor.implementation_status = "real_modem_prototype";
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
                   " ISSUE-0032 surrogate encode not implemented; "
                   "not_real_modem=true downselect_valid=false "
                   "performance_valid=false";
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
                   " ISSUE-0032 surrogate decode not implemented; "
                   "not_real_modem=true downselect_valid=false "
                   "performance_valid=false";
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

class BalancedPrototypeMode final : public IMode {
public:
  explicit BalancedPrototypeMode(ModeDescriptor descriptor)
      : descriptor_(std::move(descriptor)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  bool configure(const ModeRuntimeConfig &config) override {
    configured_ = config.sample_rate_hz == descriptor_.sample_rate_hz;
    return configured_;
  }

  EncodeResult encode(const AudioBlock &audio) override {
    EncodeResult result;
    const auto frame_index = frame_index_++;
    result.status.frame_index = frame_index;
    result.symbols.sample_rate_hz = audio.sample_rate_hz;
    result.symbols.start_time_s = audio.start_time_s;
    if (!configured_) {
      result.error = descriptor_.mode_id + " not configured";
      return result;
    }
    if (audio.sample_rate_hz != descriptor_.sample_rate_hz) {
      result.error = descriptor_.mode_id + " invalid sample rate";
      return result;
    }
    const auto input =
        make_700f_a_minimal_qpsk_input_frame(audio, frame_index, frame_index);
    const auto symbols = encode_700f_a_minimal_qpsk_symbols(input);
    const auto baseband = modulate_700f_a_minimal_qpsk_baseband(symbols);
    if (!input.descriptor.valid_rates() || !symbols.symbol_rate.valid() ||
        !baseband.sample_rate.valid()) {
      result.error =
          descriptor_.mode_id + " invalid sample/symbol rate for prototype";
      return result;
    }
    result.ok = true;
    result.status.sync = true;
    result.status.fec_ok = true;
    result.status.confidence = 0.65F;
    result.symbols.sample_rate_hz = baseband.sample_rate.hz;
    result.symbols.center_frequency_hz = baseband.center_frequency_hz;
    result.symbols.iq = baseband.samples;
    result.bits.frame_index = frame_index;
    result.bits.bits = payload_bytes_to_bits(input.payload_bytes);
    last_encoded_audio_samples_ = audio.mono.size();
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
    if (symbols.sample_rate_hz == 0) {
      result.error = descriptor_.mode_id + " invalid sample rate";
      return result;
    }
    BasebandFrame baseband;
    baseband.descriptor = make_minimal_qpsk_descriptor(
        descriptor_.mode_id, result.status.frame_index, symbols.sample_rate_hz,
        result.status.frame_index);
    baseband.sample_rate = baseband.descriptor.sample_rate;
    baseband.symbol_rate = baseband.descriptor.symbol_rate;
    baseband.occupied_bandwidth_hz = k700fAOccupiedBandwidthHz;
    baseband.center_frequency_hz = symbols.center_frequency_hz;
    baseband.samples = symbols.iq;
    baseband.modem_family = "minimal_qpsk";
    baseband.sync_family = "none";
    baseband.limitations = minimal_qpsk_limitations();
    const auto output = demodulate_700f_a_minimal_qpsk_baseband(
        baseband, last_encoded_audio_samples_);
    if (output.status.status == "failed") {
      result.error = output.status.reason.empty() ? "prototype demodulate failed"
                                                  : output.status.reason;
      return result;
    }
    result.ok = true;
    result.status.sync = true;
    result.status.fec_ok = true;
    result.status.confidence = 0.65F;
    result.bits.frame_index = result.status.frame_index;
    result.bits.bits = payload_bytes_to_bits(output.decoded_payload_bytes);
    result.audio.mono = payload_bytes_to_audio(
        output.decoded_payload_bytes, last_encoded_audio_samples_);
    return result;
  }

  void reset() noexcept override {
    frame_index_ = 0;
    last_encoded_audio_samples_ = 0;
  }

private:
  ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
  std::uint64_t last_encoded_audio_samples_ = 0;
};

class CandidateProfileFactory final : public IModeFactory {
public:
  explicit CandidateProfileFactory(ModeDescriptor descriptor)
      : descriptor_(std::move(descriptor)) {}

  const ModeDescriptor &descriptor() const noexcept override {
    return descriptor_;
  }

  std::unique_ptr<IMode> create() const override {
    if (descriptor_.mode_id == "freedv700f_a_balanced") {
      return std::make_unique<BalancedPrototypeMode>(descriptor_);
    }
    return std::make_unique<CandidateProfileMode>(descriptor_);
  }

private:
  ModeDescriptor descriptor_;
};

} // namespace

ModemInputFrame make_700f_a_minimal_qpsk_input_frame(
    const AudioBlock &audio, std::uint64_t frame_index, Seed seed) {
  ModemInputFrame input;
  input.descriptor = make_minimal_qpsk_descriptor(
      "freedv700f_a_balanced", frame_index, audio.sample_rate_hz, seed);
  input.mode_id = input.descriptor.mode_id;
  input.frame_index = frame_index;
  input.sample_rate = input.descriptor.sample_rate;
  input.payload_bytes = audio_to_payload_bytes(audio);
  input.seed = seed;
  input.requested_modem_family = "minimal_qpsk";
  input.requested_fec_family = "none";
  input.requested_sync_family = "none";
  input.requested_codec_family = "synthetic";
  return input;
}

SymbolBlock encode_700f_a_minimal_qpsk_symbols(
    const ModemInputFrame &input) {
  SymbolBlock block;
  block.descriptor = input.descriptor;
  block.modem_family = "minimal_qpsk";
  block.symbol_rate = input.descriptor.symbol_rate;
  block.bits_per_symbol = 2;
  block.fec_family = "none";
  block.sync_family = "none";
  block.limitations = minimal_qpsk_limitations();
  if (!input.descriptor.valid_rates()) {
    block.limitations.insert(block.limitations.begin(),
                             "invalid sample/symbol rate rejected");
    return block;
  }

  const auto bits = payload_bytes_to_bits(input.payload_bytes);
  block.symbols.reserve((bits.size() + 1) / 2);
  for (std::size_t offset = 0; offset < bits.size(); offset += 2) {
    const auto first = bits[offset];
    const auto second = offset + 1 < bits.size() ? bits[offset + 1] : 0U;
    block.symbols.push_back(qpsk_symbol(first, second));
  }
  return block;
}

BasebandFrame modulate_700f_a_minimal_qpsk_baseband(
    const SymbolBlock &symbols) {
  BasebandFrame baseband;
  baseband.descriptor = symbols.descriptor;
  baseband.sample_rate = symbols.descriptor.sample_rate;
  baseband.occupied_bandwidth_hz = k700fAOccupiedBandwidthHz;
  baseband.center_frequency_hz = 0.0;
  baseband.symbol_rate = symbols.symbol_rate;
  baseband.modem_family = "minimal_qpsk";
  baseband.sync_family = "none";
  baseband.limitations = minimal_qpsk_limitations();
  if (!symbols.descriptor.valid_rates() || !symbols.symbol_rate.valid()) {
    baseband.limitations.insert(baseband.limitations.begin(),
                                "invalid sample/symbol rate rejected");
    return baseband;
  }
  baseband.samples = symbols.symbols;
  return baseband;
}

ModemOutputFrame demodulate_700f_a_minimal_qpsk_baseband(
    const BasebandFrame &baseband, std::uint64_t expected_audio_samples) {
  ModemOutputFrame output;
  output.descriptor = baseband.descriptor;
  output.mode_id = baseband.descriptor.mode_id;
  output.frame_index = baseband.descriptor.frame_index;
  output.status.implementation_status = "real_modem_prototype";
  output.status.prototype = true;
  output.status.not_final_modem = true;
  output.status.downselect_valid = false;
  output.status.not_downselect_valid = true;
  output.status.performance_valid = "limited";
  output.status.limitations = minimal_qpsk_limitations();
  output.limitations = minimal_qpsk_limitations();
  output.prototype_metrics = {
      "prototype_frame_status=completed",
      "prototype_sync_status=none",
      "performance_valid=limited",
      "downselect_valid=false"};

  if (!baseband.sample_rate.valid() || !baseband.symbol_rate.valid()) {
    output.status.status = "failed";
    output.status.reason = "invalid sample/symbol rate rejected";
    output.status.limitations.insert(output.status.limitations.begin(),
                                     output.status.reason);
    output.limitations.insert(output.limitations.begin(), output.status.reason);
    return output;
  }

  const auto bits = qpsk_symbols_to_bits(baseband.samples);
  auto payload = bits_to_payload_bytes(bits);
  const auto expected_payload_bytes =
      expected_audio_samples == 0
          ? payload.size()
          : static_cast<std::size_t>(expected_audio_samples * 2U);
  if (payload.size() > expected_payload_bytes) {
    payload.resize(expected_payload_bytes);
  } else if (payload.size() < expected_payload_bytes) {
    payload.resize(expected_payload_bytes, 0U);
  }

  output.status.status = "completed";
  output.status.reason =
      "minimal QPSK-like baseband prototype completed; limited diagnostics only";
  output.decoded_payload_bytes = std::move(payload);
  output.symbol_count = baseband.samples.size();
  return output;
}

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

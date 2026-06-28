#pragma once
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace f700f {

using SampleRateHz = std::uint32_t;
using Hz = double;
using Seconds = double;
using Decibels = double;
using Seed = std::uint64_t;
using ModeId = std::string;

struct Complex32 { float re = 0.0F; float im = 0.0F; };
struct SampleRate {
  SampleRateHz hz = 0;
  [[nodiscard]] bool valid() const noexcept { return hz > 0; }
};
struct SymbolRate {
  double baud = 0.0;
  [[nodiscard]] bool valid() const noexcept {
    return std::isfinite(baud) && baud > 0.0;
  }
};
struct AudioBlock { SampleRateHz sample_rate_hz = 0; std::vector<float> mono; double start_time_s = 0.0; };
struct ComplexBlock { SampleRateHz sample_rate_hz = 0; std::vector<Complex32> iq; double center_frequency_hz = 0.0; double start_time_s = 0.0; };
struct BitBlock { std::vector<std::uint8_t> bits; std::uint64_t frame_index = 0; };
struct SoftBitBlock { std::vector<float> llr; std::uint64_t frame_index = 0; };
struct FrameStatus { std::uint64_t frame_index = 0; bool sync = false; bool fec_ok = false; float confidence = 0.0F; std::uint32_t corrected_errors = 0; std::uint32_t erasures = 0; };
struct Capability { std::string key; std::string value; };
struct ModuleDescriptor { std::string module_id; std::string module_name; std::string module_version; std::string abi_version; std::vector<Capability> capabilities; };

struct CarrierLayout {
  std::string family = "placeholder";
  std::uint32_t carrier_count = 0;
  double center_frequency_hz = 0.0;
  double occupied_bandwidth_hz = 0.0;
  std::vector<std::string> limitations;
};

struct TimingSyncStatus {
  std::string family = "placeholder";
  bool locked = false;
  double timing_error_s = 0.0;
  std::vector<std::string> limitations;
};

struct FecStatus {
  std::string family = "placeholder";
  bool enabled = false;
  bool decoded = false;
  std::uint32_t corrected_errors = 0;
  std::uint32_t erasures = 0;
  std::vector<std::string> limitations;
};

struct CodecStatus {
  std::string family = "placeholder";
  bool encoded = false;
  bool decoded = false;
  std::vector<std::string> limitations;
};

struct PrototypeFrameStatus {
  std::string status = "limited";
  std::string reason;
  std::string implementation_status = "real_modem_prototype";
  bool prototype = true;
  bool not_final_modem = true;
  bool downselect_valid = false;
  bool not_downselect_valid = true;
  std::string performance_valid = "limited";
  std::vector<std::string> limitations;
};

struct ModemFrameDescriptor {
  ModeId mode_id;
  std::uint64_t frame_index = 0;
  SampleRate sample_rate;
  SymbolRate symbol_rate;
  Seed seed = 0;
  std::string modem_family = "minimal_qpsk_baseband";
  CarrierLayout carrier_layout;
  TimingSyncStatus sync;
  FecStatus fec;
  CodecStatus codec;
  std::vector<std::string> limitations;

  [[nodiscard]] bool valid_rates() const noexcept {
    return sample_rate.valid() && symbol_rate.valid();
  }
};

struct ModemInputFrame {
  ModemFrameDescriptor descriptor;
  ModeId mode_id;
  std::uint64_t frame_index = 0;
  SampleRate sample_rate;
  std::vector<std::uint8_t> payload_bytes;
  Seed seed = 0;
  std::string requested_modem_family;
  std::string requested_fec_family;
  std::string requested_sync_family;
  std::string requested_codec_family;
};

struct SymbolBlock {
  ModemFrameDescriptor descriptor;
  std::string modem_family;
  SymbolRate symbol_rate;
  std::vector<Complex32> symbols;
  std::uint32_t bits_per_symbol = 0;
  std::string fec_family;
  std::string sync_family;
  std::vector<std::string> limitations;
};

struct BasebandFrame {
  ModemFrameDescriptor descriptor;
  SampleRate sample_rate;
  double occupied_bandwidth_hz = 0.0;
  double center_frequency_hz = 0.0;
  std::vector<Complex32> samples;
  SymbolRate symbol_rate;
  std::string modem_family;
  std::string sync_family;
  std::vector<std::string> limitations;
};

struct ModemOutputFrame {
  ModemFrameDescriptor descriptor;
  ModeId mode_id;
  std::uint64_t frame_index = 0;
  std::vector<std::uint8_t> decoded_payload_bytes;
  PrototypeFrameStatus status;
  std::uint64_t symbol_error_count = 0;
  std::uint64_t symbol_count = 0;
  std::vector<std::string> prototype_metrics;
  std::vector<std::string> limitations;
};

struct ModeCapabilities {
  bool audio_input = false;
  bool audio_output = false;
  bool complex_input = false;
  bool complex_output = false;
  bool bit_payload = false;
  bool soft_bits = false;
  bool variable_frame_size = false;
};

struct ModeDescriptor {
  ModeId mode_id;
  std::string display_name;
  SampleRateHz sample_rate_hz = 0;
  double rf_bandwidth_hz = 0.0;
  double audio_low_hz = 300.0;
  double audio_high_hz = 3300.0;
  double nominal_latency_s = 0.0;
  double frame_duration_s = 0.0;
  double cp_duration_s = 0.0;
  double audio_bandwidth_hz = 0.0;
  std::uint32_t raw_bitrate_bps = 0;
  std::uint32_t voice_bitrate_bps = 0;
  std::uint32_t text_bitrate_bps = 0;
  std::uint32_t carrier_count = 0;
  std::string codec_id;
  std::string fec_id;
  std::string modem_id;
  std::string modulation_family;
  std::string pilot_strategy;
  bool official_baseline = false;
  bool emulator = false;
  std::string implementation_status;
  ModeCapabilities capabilities;
};

struct ModeRuntimeConfig {
  SampleRateHz sample_rate_hz = 0;
  std::uint32_t max_frame_samples = 0;
  std::vector<Capability> parameters;
};

struct EncodeResult {
  bool ok = false;
  std::string error;
  ComplexBlock symbols;
  BitBlock bits;
  FrameStatus status;
};

struct DecodeResult {
  bool ok = false;
  std::string error;
  AudioBlock audio;
  BitBlock bits;
  FrameStatus status;
};

class IMode {
public:
  virtual ~IMode() = default;
  virtual const ModeDescriptor &descriptor() const noexcept = 0;
  virtual bool configure(const ModeRuntimeConfig &config) = 0;
  virtual EncodeResult encode(const AudioBlock &audio) = 0;
  virtual DecodeResult decode(const ComplexBlock &symbols) = 0;
  virtual void reset() noexcept = 0;
};

class IModeFactory {
public:
  virtual ~IModeFactory() = default;
  virtual const ModeDescriptor &descriptor() const noexcept = 0;
  virtual std::unique_ptr<IMode> create() const = 0;
};

class ModeRegistry {
public:
  bool register_factory(std::shared_ptr<IModeFactory> factory) {
    if (!factory || factory->descriptor().mode_id.empty()) {
      return false;
    }
    const auto mode_id = factory->descriptor().mode_id;
    return factories_.emplace(mode_id, std::move(factory)).second;
  }

  const ModeDescriptor *find(const ModeId &mode_id) const noexcept {
    const auto it = factories_.find(mode_id);
    if (it == factories_.end()) {
      return nullptr;
    }
    return &it->second->descriptor();
  }

  std::unique_ptr<IMode> create(const ModeId &mode_id) const {
    const auto it = factories_.find(mode_id);
    if (it == factories_.end()) {
      return nullptr;
    }
    return it->second->create();
  }

  std::vector<ModeDescriptor> descriptors() const {
    std::vector<ModeDescriptor> result;
    result.reserve(factories_.size());
    for (const auto &[mode_id, factory] : factories_) {
      (void)mode_id;
      result.push_back(factory->descriptor());
    }
    return result;
  }

private:
  std::unordered_map<ModeId, std::shared_ptr<IModeFactory>> factories_;
};

} // namespace f700f

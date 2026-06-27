#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace f700f {
using SampleRateHz = std::uint32_t; using Hz = double; using Seconds = double; using Decibels = double; using Seed = std::uint64_t;
struct Complex32 { float re = 0.0F; float im = 0.0F; };
struct AudioBlock { SampleRateHz sample_rate_hz = 0; std::vector<float> mono; double start_time_s = 0.0; };
struct ComplexBlock { SampleRateHz sample_rate_hz = 0; std::vector<Complex32> iq; double center_frequency_hz = 0.0; double start_time_s = 0.0; };
struct BitBlock { std::vector<std::uint8_t> bits; std::uint64_t frame_index = 0; };
struct SoftBitBlock { std::vector<float> llr; std::uint64_t frame_index = 0; };
struct FrameStatus { std::uint64_t frame_index = 0; bool sync = false; bool fec_ok = false; float confidence = 0.0F; std::uint32_t corrected_errors = 0; std::uint32_t erasures = 0; };
struct Capability { std::string key; std::string value; };
struct ModuleDescriptor { std::string module_id; std::string module_name; std::string module_version; std::string abi_version; std::vector<Capability> capabilities; };
struct ModeDescriptor { std::string mode_id; double rf_bandwidth_hz = 0.0; double audio_low_hz = 300.0; double audio_high_hz = 3300.0; double nominal_latency_s = 0.0; double frame_duration_s = 0.0; std::uint32_t raw_bitrate_bps = 0; std::uint32_t voice_bitrate_bps = 0; std::uint32_t text_bitrate_bps = 0; std::string codec_id; std::string fec_id; std::string modem_id; };
} // namespace f700f

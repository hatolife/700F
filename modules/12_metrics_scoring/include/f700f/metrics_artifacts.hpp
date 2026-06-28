#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <f700f/types.hpp>

namespace f700f::metrics {

struct ModeDescriptorSnapshot {
  std::string mode_id;
  std::string display_name;
  std::uint32_t sample_rate_hz = 0;
  double rf_bandwidth_hz = 0.0;
  double audio_low_hz = 0.0;
  double audio_high_hz = 0.0;
  double nominal_latency_s = 0.0;
  double frame_duration_s = 0.0;
  std::uint32_t raw_bitrate_bps = 0;
  std::uint32_t voice_bitrate_bps = 0;
  std::string codec_id;
  std::string fec_id;
  std::string modem_id;
  double audio_bandwidth_hz = 0.0;
  bool official_baseline = false;
  bool emulator = false;
  std::string implementation_status;
  std::string implementation_classification;
  bool not_real_modem = false;
  bool downselect_valid = true;
  bool not_downselect_valid = false;
  bool performance_valid = true;
  std::string performance_validity = "valid";
  std::string downselect_validity = "valid";
  bool prototype = false;
  bool not_final_modem = false;
  bool waveform_capable = false;
  std::string codec_family;
  std::string fec_family;
  std::string sync_family;
  std::string modem_family;
  std::string prototype_limitations;
  std::string prototype_warning;
  std::string surrogate_model_name;
  std::string surrogate_model_version;
  std::string surrogate_limitations;
  bool supports_audio_input = false;
  bool supports_audio_output = false;
  bool supports_complex_input = false;
  bool supports_complex_output = false;
  bool supports_bit_payload = false;
};

struct ResultArtifact {
  // project metadata
  std::string project_version;
  std::string module_version;
  std::string run_id;

  // mode and channel context
  ModeDescriptorSnapshot mode_descriptor;
  std::string channel_id;
  std::map<std::string, std::string> channel_parameters;

  // run seed and channeling parameters
  std::uint64_t seed = 0;
  double snr_db = 0.0;
  double freq_offset_hz = 0.0;

  // core result metrics
  std::uint64_t frame_count = 0;
  std::uint64_t sample_count = 0;
  std::optional<double> ber;
  std::optional<double> fer;
  std::uint64_t sync_loss_count = 0;
  double dropout_rate = 0.0;
  double latency_estimate_s = 0.0;
  std::string audio_export_path;
  std::vector<std::string> warnings;
  std::optional<std::string> skipped_reason;
  std::optional<std::string> error_summary;
  std::optional<double> prototype_symbol_error_rate;
  std::string prototype_frame_status;
  std::string prototype_sync_status;
  std::uint64_t prototype_baseband_sample_count = 0;
  std::string prototype_limitations;
  std::map<std::string, std::string> optional_metrics;
};

ModeDescriptorSnapshot make_mode_descriptor_snapshot(const f700f::ModeDescriptor &descriptor);
ResultArtifact make_empty_result_artifact();

std::string to_json(const ResultArtifact &result);
std::string to_csv_header(const std::vector<std::string> &extra_columns = {});
std::string to_csv_row(const ResultArtifact &result,
                       const std::vector<std::string> &extra_columns = {});

ResultArtifact from_json(const std::string &json_payload);
ResultArtifact from_csv_row(const std::string &header_line,
                           const std::string &row_line);

std::vector<std::string> metric_column_names(const ResultArtifact &result);

}  // namespace f700f::metrics

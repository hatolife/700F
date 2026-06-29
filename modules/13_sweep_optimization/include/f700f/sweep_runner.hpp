#pragma once

#include <f700f/simulation_pipeline.hpp>

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace f700f {

struct SweepModeConfig {
  ModeId mode_id;
  bool skip_if_unavailable = true;
};

struct SweepChannelCondition {
  std::string condition_id;
  std::vector<ChannelConfig> channel_chain;
};

struct SweepConfig {
  std::string run_id_prefix;
  std::string output_directory;
  SimulationInputConfig input = GeneratedToneConfig{};
  bool export_audio = false;
  std::vector<std::string> metric_ids;
  std::vector<SweepModeConfig> modes;
  std::vector<SweepChannelCondition> channel_conditions;
  std::vector<Seed> seeds;
};

struct SweepConfigOverrides {
  std::optional<std::string> output_directory;
  std::optional<std::string> run_id_prefix;
};

enum class SweepRunStatus {
  Completed,
  Skipped,
  Failed,
};

struct SweepRunRecord {
  SweepRunStatus status = SweepRunStatus::Failed;
  std::string run_id;
  ModeId mode_id;
  std::string condition_id;
  Seed seed = 0;
  std::string audio_export_path = "N/A";
  SimulationResult simulation;
  std::string skipped_reason;
  std::string error_summary;
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
  std::string prototype_symbol_error_rate;
  std::string prototype_frame_status;
  std::string prototype_sync_status;
  std::uint64_t prototype_baseband_sample_count = 0;
  std::string occupied_bandwidth_estimate_hz;
  std::string occupied_bandwidth_low_hz;
  std::string occupied_bandwidth_high_hz;
  std::string occupied_bandwidth_ratio;
  std::string occupied_bandwidth_status;
  std::string surrogate_model_name;
  std::string surrogate_model_version;
  std::string surrogate_limitations;
  std::string surrogate_readiness_score_synthetic;
  std::string synthetic_metrics_label;
};

struct SweepArtifact {
  std::string artifact_id;
  std::string path;
  std::string media_type;
};

struct SweepResult {
  bool ok = false;
  std::string run_id_prefix;
  std::vector<SweepRunRecord> records;
  std::vector<SweepArtifact> artifacts;
  std::string error;
};

class SweepRunner {
public:
  bool register_mode_factory(std::shared_ptr<IModeFactory> factory);
  bool register_channel_factory(std::shared_ptr<IChannelFactory> factory);
  bool register_metric(std::shared_ptr<IMetric> metric);

  SweepResult run(const SweepConfig &config) const;

private:
  SimulationRunner simulation_runner_;
  std::unordered_set<ModeId> available_modes_;
  std::unordered_map<ModeId, ModeDescriptor> available_mode_descriptors_;
};

std::string sweep_run_status_name(SweepRunStatus status);
std::string sweep_result_to_json(const SweepResult &result);
std::string sweep_result_to_csv(const SweepResult &result);

std::shared_ptr<IChannelFactory> make_awgn_channel_factory();
std::shared_ptr<IChannelFactory> make_frequency_offset_channel_factory();
std::shared_ptr<IChannelFactory> make_simple_gain_fading_channel_factory();

SweepConfig load_sweep_config_from_file(const std::string &path,
                                        std::string &error);
SweepConfig load_sweep_config_from_file(const std::string &path,
                                        const SweepConfigOverrides &overrides,
                                        std::string &error);

SweepConfig make_m1_baseline_smoke_sweep_config(std::string output_directory);
SweepConfig make_m2_channel_matrix_smoke_sweep_config(std::string output_directory);
SweepConfig make_m2_channel_matrix_full_sweep_config(std::string output_directory);
SweepConfig make_m2_700f_candidate_smoke_sweep_config(
    std::string output_directory);
SweepConfig make_m2_700f_candidate_full_sweep_config(
    std::string output_directory);
SweepConfig make_m3_channel_impairment_smoke_sweep_config(
    std::string output_directory);
void register_m2_campaign_mode_factories(SweepRunner &runner);

} // namespace f700f

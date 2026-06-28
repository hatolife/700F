#pragma once

#include <f700f/simulation_pipeline.hpp>

#include <string>
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
  SimulationResult simulation;
  std::string skipped_reason;
  std::string error_summary;
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
};

std::string sweep_run_status_name(SweepRunStatus status);
std::string sweep_result_to_json(const SweepResult &result);
std::string sweep_result_to_csv(const SweepResult &result);

std::shared_ptr<IChannelFactory> make_awgn_channel_factory();
std::shared_ptr<IChannelFactory> make_frequency_offset_channel_factory();
std::shared_ptr<IChannelFactory> make_simple_gain_fading_channel_factory();

SweepConfig make_m1_baseline_smoke_sweep_config(std::string output_directory);
SweepConfig make_m2_channel_matrix_smoke_sweep_config(std::string output_directory);
SweepConfig make_m2_channel_matrix_full_sweep_config(std::string output_directory);
SweepConfig make_m2_700f_candidate_smoke_sweep_config(
    std::string output_directory);
SweepConfig make_m2_700f_candidate_full_sweep_config(
    std::string output_directory);

} // namespace f700f

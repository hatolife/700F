#pragma once

#include <f700f/types.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace f700f {

struct GeneratedToneConfig {
  SampleRateHz sample_rate_hz = 8000;
  std::uint32_t sample_count = 0;
  float frequency_hz = 1000.0F;
  float amplitude = 0.25F;
};

struct AudioFileInputConfig {
  std::string path;
  SampleRateHz sample_rate_hz = 8000;
};

using SimulationInputConfig = std::variant<GeneratedToneConfig, AudioFileInputConfig>;

struct ChannelConfig {
  std::string channel_id;
  std::vector<Capability> parameters;
};

struct SimulationConfig {
  std::string run_id;
  Seed seed = 0;
  SimulationInputConfig input = GeneratedToneConfig{};
  ModeId mode_id;
  std::vector<ChannelConfig> channel_chain;
  std::string output_directory;
  bool export_audio = false;
  std::vector<std::string> metric_ids;
};

enum class PipelineStage {
  Input,
  Encode,
  Channel,
  Decode,
  Metrics,
};

struct StageStatus {
  PipelineStage stage = PipelineStage::Input;
  bool ok = false;
  std::string message;
};

struct MetricValue {
  std::string metric_id;
  double value = 0.0;

  bool operator==(const MetricValue &) const = default;
};

struct ResultArtifact {
  std::string artifact_id;
  std::string path;
  std::string media_type;

  bool operator==(const ResultArtifact &) const = default;
};

struct SimulationResult {
  bool ok = false;
  std::string run_id;
  Seed seed = 0;
  ModeId mode_id;
  std::vector<std::string> channel_ids;
  std::string deterministic_digest;
  std::string audio_export_path = "N/A";
  AudioBlock input_audio;
  ComplexBlock channel_output;
  AudioBlock decoded_audio;
  std::vector<MetricValue> metrics;
  std::vector<StageStatus> stage_statuses;
  std::vector<ResultArtifact> artifacts;
  std::string error;

  const StageStatus &stage_status(PipelineStage stage) const noexcept;
};

class IChannel {
public:
  virtual ~IChannel() = default;
  virtual std::string channel_id() const = 0;
  virtual bool configure(const ChannelConfig &config, Seed seed, std::string &error) = 0;
  virtual ComplexBlock apply(const ComplexBlock &input, std::string &error) = 0;
};

class IChannelFactory {
public:
  virtual ~IChannelFactory() = default;
  virtual std::string channel_id() const = 0;
  virtual std::unique_ptr<IChannel> create() const = 0;
};

class IMetric {
public:
  virtual ~IMetric() = default;
  virtual std::string metric_id() const = 0;
  virtual bool evaluate(const AudioBlock &reference, const AudioBlock &decoded, MetricValue &metric, std::string &error) const = 0;
};

class SimulationRunner {
public:
  bool register_mode_factory(std::shared_ptr<IModeFactory> factory);
  bool register_channel_factory(std::shared_ptr<IChannelFactory> factory);
  bool register_metric(std::shared_ptr<IMetric> metric);
  SimulationResult run(const SimulationConfig &config) const;

private:
  ModeRegistry modes_;
  std::unordered_map<std::string, std::shared_ptr<IChannelFactory>> channel_factories_;
  std::unordered_map<std::string, std::shared_ptr<IMetric>> metrics_;
};

std::shared_ptr<IModeFactory> make_dummy_mode_factory(ModeId mode_id);
std::shared_ptr<IChannelFactory> make_identity_channel_factory();
std::shared_ptr<IMetric> make_dummy_metric();

std::string pipeline_stage_name(PipelineStage stage);
std::string simulation_result_to_json(const SimulationResult &result);
std::string simulation_result_to_csv(const SimulationResult &result);

} // namespace f700f

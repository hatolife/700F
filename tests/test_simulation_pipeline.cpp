#include <f700f/simulation_pipeline.hpp>

#include <cassert>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace {

f700f::SimulationConfig make_config() {
  f700f::SimulationConfig config;
  config.run_id = "run-001";
  config.seed = 12345;
  config.input = f700f::GeneratedToneConfig{.sample_rate_hz = 8000, .sample_count = 16, .frequency_hz = 1000.0F, .amplitude = 0.25F};
  config.mode_id = "dummy.mode";
  config.channel_chain.push_back({.channel_id = "identity"});
  config.output_directory = "build/test-artifacts";
  config.export_audio = true;
  config.metric_ids = {"dummy.metric"};
  return config;
}

f700f::SimulationRunner make_runner() {
  f700f::SimulationRunner runner;
  runner.register_mode_factory(f700f::make_dummy_mode_factory("dummy.mode"));
  runner.register_channel_factory(f700f::make_identity_channel_factory());
  runner.register_metric(f700f::make_dummy_metric());
  return runner;
}

void dummy_mode_identity_channel_succeeds() {
  auto runner = make_runner();
  const auto result = runner.run(make_config());
  assert(result.ok);
  assert(result.stage_statuses.size() == 5);
  assert(result.stage_status(f700f::PipelineStage::Input).ok);
  assert(result.stage_status(f700f::PipelineStage::Encode).ok);
  assert(result.stage_status(f700f::PipelineStage::Channel).ok);
  assert(result.stage_status(f700f::PipelineStage::Decode).ok);
  assert(result.stage_status(f700f::PipelineStage::Metrics).ok);
  assert(!result.metrics.empty());
  assert(result.artifacts.size() == 3);
  assert(std::filesystem::exists("build/test-artifacts/run-001.json"));
  assert(std::filesystem::exists("build/test-artifacts/run-001.csv"));
}

void same_seed_gives_same_result() {
  auto runner = make_runner();
  const auto first = runner.run(make_config());
  const auto second = runner.run(make_config());
  assert(first.ok);
  assert(second.ok);
  assert(first.deterministic_digest == second.deterministic_digest);
  assert(first.decoded_audio.mono == second.decoded_audio.mono);
  assert(first.metrics == second.metrics);
}

void invalid_mode_id_fails() {
  auto runner = make_runner();
  auto config = make_config();
  config.mode_id = "missing.mode";
  const auto result = runner.run(config);
  assert(!result.ok);
  assert(!result.stage_status(f700f::PipelineStage::Encode).ok);
  assert(result.stage_status(f700f::PipelineStage::Encode).message.find("missing.mode") != std::string::npos);
}

void invalid_channel_config_fails() {
  auto runner = make_runner();
  auto config = make_config();
  config.channel_chain.front().parameters.push_back({"fail", "true"});
  const auto result = runner.run(config);
  assert(!result.ok);
  assert(!result.stage_status(f700f::PipelineStage::Channel).ok);
}

void zero_length_audio_does_not_crash() {
  auto runner = make_runner();
  auto config = make_config();
  config.input = f700f::GeneratedToneConfig{.sample_rate_hz = 8000, .sample_count = 0, .frequency_hz = 1000.0F, .amplitude = 0.25F};
  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.decoded_audio.mono.empty());
}

void result_contains_identity_metadata() {
  auto runner = make_runner();
  const auto result = runner.run(make_config());
  assert(result.run_id == "run-001");
  assert(result.seed == 12345);
  assert(result.mode_id == "dummy.mode");
  assert(result.channel_ids == std::vector<std::string>({"identity"}));
}

void pipeline_stage_failure_is_reflected() {
  auto runner = make_runner();
  auto config = make_config();
  config.metric_ids = {"missing.metric"};
  const auto result = runner.run(config);
  assert(!result.ok);
  assert(result.stage_status(f700f::PipelineStage::Metrics).stage == f700f::PipelineStage::Metrics);
  assert(!result.stage_status(f700f::PipelineStage::Metrics).ok);
}

} // namespace

int main() {
  dummy_mode_identity_channel_succeeds();
  same_seed_gives_same_result();
  invalid_mode_id_fails();
  invalid_channel_config_fails();
  zero_length_audio_does_not_crash();
  result_contains_identity_metadata();
  pipeline_stage_failure_is_reflected();
  return 0;
}

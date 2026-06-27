#include <f700f/sweep_runner.hpp>

#include <cassert>
#include <filesystem>
#include <string>
#include <vector>

namespace {

f700f::SweepConfig make_sweep_config() {
  f700f::SweepConfig config;
  config.run_id_prefix = "sweep";
  config.output_directory = "build/test-artifacts/sweep";
  config.input = f700f::GeneratedToneConfig{.sample_rate_hz = 8000,
                                            .sample_count = 16,
                                            .frequency_hz = 1000.0F,
                                            .amplitude = 0.25F};
  config.metric_ids = {"dummy.metric"};
  config.modes = {{.mode_id = "dummy.mode"}};
  config.channel_conditions = {{.condition_id = "identity",
                                .channel_chain = {{.channel_id = "identity"}}}};
  config.seeds = {7};
  return config;
}

f700f::SweepRunner make_runner() {
  f700f::SweepRunner runner;
  runner.register_mode_factory(f700f::make_dummy_mode_factory("dummy.mode"));
  runner.register_channel_factory(f700f::make_identity_channel_factory());
  runner.register_metric(f700f::make_dummy_metric());
  return runner;
}

void empty_sweep_config_rejected() {
  auto runner = make_runner();
  const auto result = runner.run({});
  assert(!result.ok);
  assert(result.records.empty());
  assert(result.error.find("mode") != std::string::npos);
}

void single_mode_channel_seed_gives_one_result() {
  auto runner = make_runner();
  const auto result = runner.run(make_sweep_config());
  assert(result.ok);
  assert(result.records.size() == 1);
  assert(result.records[0].status == f700f::SweepRunStatus::Completed);
  assert(result.records[0].simulation.ok);
}

void two_seeds_give_two_results() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  config.seeds = {7, 8};
  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 2);
  assert(result.records[0].seed == 7);
  assert(result.records[1].seed == 8);
}

void skipped_mode_recorded() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  config.modes = {{.mode_id = "missing.mode"}};
  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 1);
  assert(result.records[0].status == f700f::SweepRunStatus::Skipped);
  assert(result.records[0].skipped_reason.find("missing.mode") != std::string::npos);
}

void failed_run_does_not_stop_other_runs() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  config.channel_conditions = {
      {.condition_id = "broken",
       .channel_chain = {{.channel_id = "identity",
                          .parameters = {{"fail", "true"}}}}},
      {.condition_id = "identity",
       .channel_chain = {{.channel_id = "identity"}}}};
  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 2);
  assert(result.records[0].status == f700f::SweepRunStatus::Failed);
  assert(result.records[1].status == f700f::SweepRunStatus::Completed);
}

void json_and_csv_generated() {
  auto runner = make_runner();
  const auto result = runner.run(make_sweep_config());
  assert(result.ok);
  assert(result.artifacts.size() == 2);
  assert(std::filesystem::exists("build/test-artifacts/sweep/sweep.json"));
  assert(std::filesystem::exists("build/test-artifacts/sweep/sweep.csv"));
}

void stable_ordering_for_same_config() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  config.modes = {{.mode_id = "dummy.mode"}, {.mode_id = "missing.mode"}};
  config.channel_conditions.push_back(
      {.condition_id = "identity-again",
       .channel_chain = {{.channel_id = "identity"}}});
  config.seeds = {2, 1};
  const auto first = runner.run(config);
  const auto second = runner.run(config);
  assert(first.ok);
  assert(second.ok);
  assert(first.records.size() == second.records.size());
  for (std::size_t i = 0; i < first.records.size(); ++i) {
    assert(first.records[i].run_id == second.records[i].run_id);
    assert(first.records[i].status == second.records[i].status);
  }
}

void ci_smoke_sweep_completes_quickly() {
  auto runner = make_runner();
  auto config = f700f::make_m1_baseline_smoke_sweep_config(
      "build/test-artifacts/m1-baseline-smoke");
  config.modes = {{.mode_id = "dummy.mode"},
                  {.mode_id = "freedv700d_official"},
                  {.mode_id = "freedv700e_official"}};
  config.channel_conditions = {{.condition_id = "identity",
                                .channel_chain = {{.channel_id = "identity"}}}};
  config.seeds = {1};
  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 3);
  assert(result.records[0].status == f700f::SweepRunStatus::Completed);
  assert(result.records[1].status == f700f::SweepRunStatus::Skipped);
  assert(result.records[2].status == f700f::SweepRunStatus::Skipped);
}

} // namespace

int main() {
  empty_sweep_config_rejected();
  single_mode_channel_seed_gives_one_result();
  two_seeds_give_two_results();
  skipped_mode_recorded();
  failed_run_does_not_stop_other_runs();
  json_and_csv_generated();
  stable_ordering_for_same_config();
  ci_smoke_sweep_completes_quickly();
  return 0;
}

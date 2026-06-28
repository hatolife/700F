#include <f700f/sweep_runner.hpp>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <set>
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
  runner.register_channel_factory(f700f::make_awgn_channel_factory());
  runner.register_channel_factory(f700f::make_frequency_offset_channel_factory());
  runner.register_channel_factory(f700f::make_simple_gain_fading_channel_factory());
  runner.register_metric(f700f::make_dummy_metric());
  return runner;
}

void assert_m2_candidate_mode_order(
    const std::vector<f700f::SweepModeConfig> &modes) {
  const std::vector<std::string> expected_modes = {
      "ssb_standard_3k",
      "ssb_narrow_1k9",
      "freedv700d_emulated",
      "freedv700e_emulated",
      "freedv700d_official",
      "freedv700e_official",
      "freedv700f_a_balanced",
      "freedv700f_b_robust",
      "freedv700f_c_quality"};
  assert(modes.size() == expected_modes.size());
  for (std::size_t i = 0; i < expected_modes.size(); ++i) {
    assert(modes[i].mode_id == expected_modes[i]);
    assert(modes[i].skip_if_unavailable);
  }
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

void m2_channel_matrix_smoke_has_required_conditions() {
  const auto config = f700f::make_m2_channel_matrix_smoke_sweep_config(
      "build/test-artifacts/m2-channel-matrix-smoke");
  assert(config.run_id_prefix == "m2-channel-matrix-smoke");
  assert(config.seeds == std::vector<f700f::Seed>{1});
  assert(config.channel_conditions.size() == 3);
  assert(config.channel_conditions[0].condition_id == "identity");
  assert(config.channel_conditions[1].condition_id == "awgn-snr-6db");
  assert(config.channel_conditions[1].channel_chain.size() == 1);
  assert(config.channel_conditions[1].channel_chain[0].channel_id == "awgn");
  assert(config.channel_conditions[1].channel_chain[0].parameters[0].value == "6.0");
  assert(config.channel_conditions[2].condition_id == "awgn-snr-0db");
  assert(config.channel_conditions[2].channel_chain.size() == 1);
  assert(config.channel_conditions[2].channel_chain[0].channel_id == "awgn");
  assert(config.channel_conditions[2].channel_chain[0].parameters[0].value == "0.0");
}

void m2_channel_matrix_full_has_unique_cartesian_conditions() {
  const auto config = f700f::make_m2_channel_matrix_full_sweep_config(
      "build/test-artifacts/m2-channel-matrix-full");
  assert(config.run_id_prefix == "m2-channel-matrix-full");
  assert(config.seeds == std::vector<f700f::Seed>({1, 2, 3}));
  assert(config.channel_conditions.size() == 72);

  std::set<std::string> ids;
  bool saw_first = false;
  bool saw_last = false;
  for (const auto &condition : config.channel_conditions) {
    assert(ids.insert(condition.condition_id).second);
    if (condition.condition_id == "awgn-snr--2db-fo-0hz-fading-none") {
      saw_first = true;
      assert(condition.channel_chain.size() == 2);
    }
    if (condition.condition_id == "awgn-snr-8db-fo-200hz-fading-medium") {
      saw_last = true;
      assert(condition.channel_chain.size() == 3);
    }
  }
  assert(saw_first);
  assert(saw_last);
}

void duplicate_condition_ids_are_rejected() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  config.channel_conditions.push_back(config.channel_conditions.front());
  const auto result = runner.run(config);
  assert(!result.ok);
  assert(result.records.empty());
  assert(result.error.find("duplicate channel condition id") != std::string::npos);
}

void invalid_channel_matrix_parameters_are_rejected() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  config.channel_conditions = {
      {.condition_id = "invalid-snr",
       .channel_chain = {{.channel_id = "awgn",
                          .parameters = {{"snr_db", "not-a-number"}}}}},
      {.condition_id = "invalid-frequency-offset",
       .channel_chain = {{.channel_id = "frequency_offset",
                          .parameters = {{"freq_offset_hz", "-50.0"}}}}}};

  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 2);
  assert(result.records[0].status == f700f::SweepRunStatus::Failed);
  assert(result.records[0].error_summary.find("snr_db") != std::string::npos);
  assert(result.records[1].status == f700f::SweepRunStatus::Failed);
  assert(result.records[1].error_summary.find("freq_offset_hz") != std::string::npos);
}

void m2_channel_matrix_empty_seed_list_rejected() {
  auto runner = make_runner();
  auto config = f700f::make_m2_channel_matrix_smoke_sweep_config(
      "build/test-artifacts/m2-channel-matrix-smoke");
  config.modes = {{.mode_id = "dummy.mode"}};
  config.seeds.clear();
  const auto result = runner.run(config);
  assert(!result.ok);
  assert(result.error.find("seed") != std::string::npos);
}

void m2_700f_candidate_smoke_campaign_records_skips_and_artifacts() {
  auto runner = make_runner();
  auto config = f700f::make_m2_700f_candidate_smoke_sweep_config(
      "build/test-artifacts/m2-700f-candidate-smoke");
  config.modes.push_back({.mode_id = "unknown.mode.for-skip-test"});

  assert(config.run_id_prefix == "m2-700f-candidate-smoke");
  assert(config.channel_conditions.size() == 3);
  assert(config.seeds == std::vector<f700f::Seed>{1});
  std::vector<f700f::SweepModeConfig> campaign_modes(config.modes.begin(),
                                                     config.modes.end() - 1);
  assert_m2_candidate_mode_order(campaign_modes);

  const auto first = runner.run(config);
  const auto second = runner.run(config);
  assert(first.ok);
  assert(second.ok);
  assert(first.records.size() == config.modes.size() *
                                     config.channel_conditions.size() *
                                     config.seeds.size());
  assert(first.records.size() == second.records.size());
  for (std::size_t i = 0; i < first.records.size(); ++i) {
    assert(first.records[i].run_id == second.records[i].run_id);
    assert(first.records[i].mode_id == second.records[i].mode_id);
    assert(first.records[i].condition_id == second.records[i].condition_id);
    assert(first.records[i].seed == second.records[i].seed);
    assert(first.records[i].status == f700f::SweepRunStatus::Skipped);
    assert(first.records[i].skipped_reason.find(first.records[i].mode_id) !=
           std::string::npos);
  }

  assert(std::filesystem::exists(
      "build/test-artifacts/m2-700f-candidate-smoke/"
      "m2-700f-candidate-smoke.json"));
  assert(std::filesystem::exists(
      "build/test-artifacts/m2-700f-candidate-smoke/"
      "m2-700f-candidate-smoke.csv"));

  std::ifstream json("build/test-artifacts/m2-700f-candidate-smoke/"
                     "m2-700f-candidate-smoke.json");
  const std::string json_text((std::istreambuf_iterator<char>(json)),
                              std::istreambuf_iterator<char>());
  assert(json_text.find("unknown.mode.for-skip-test") != std::string::npos);
  assert(json_text.find("mode id not registered: unknown.mode.for-skip-test") !=
         std::string::npos);
}

void m2_700f_candidate_full_campaign_uses_full_matrix() {
  const auto config = f700f::make_m2_700f_candidate_full_sweep_config(
      "build/test-artifacts/m2-700f-candidate-full");
  assert(config.run_id_prefix == "m2-700f-candidate-full");
  assert_m2_candidate_mode_order(config.modes);
  assert(config.channel_conditions.size() == 72);
  assert(config.seeds == std::vector<f700f::Seed>({1, 2, 3}));
  assert(config.channel_conditions.front().condition_id ==
         "awgn-snr--2db-fo-0hz-fading-none");
  assert(config.channel_conditions.back().condition_id ==
         "awgn-snr-8db-fo-200hz-fading-medium");
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
  m2_channel_matrix_smoke_has_required_conditions();
  m2_channel_matrix_full_has_unique_cartesian_conditions();
  duplicate_condition_ids_are_rejected();
  invalid_channel_matrix_parameters_are_rejected();
  m2_channel_matrix_empty_seed_list_rejected();
  m2_700f_candidate_smoke_campaign_records_skips_and_artifacts();
  m2_700f_candidate_full_campaign_uses_full_matrix();
  return 0;
}

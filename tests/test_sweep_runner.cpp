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
  f700f::register_m2_campaign_mode_factories(runner);
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
  std::size_t completed = 0;
  std::size_t skipped = 0;
  std::size_t failed = 0;
  std::size_t surrogate_completed = 0;
  std::size_t emulated_surrogate_completed = 0;
  std::size_t official_completed = 0;
  std::size_t official_skipped = 0;
  std::size_t unknown_skipped = 0;
  for (std::size_t i = 0; i < first.records.size(); ++i) {
    assert(first.records[i].run_id == second.records[i].run_id);
    assert(first.records[i].mode_id == second.records[i].mode_id);
    assert(first.records[i].condition_id == second.records[i].condition_id);
    assert(first.records[i].seed == second.records[i].seed);
    if (first.records[i].status == f700f::SweepRunStatus::Completed) {
      ++completed;
    } else if (first.records[i].status == f700f::SweepRunStatus::Skipped) {
      ++skipped;
    } else {
      ++failed;
    }
    if (first.records[i].mode_id.find("freedv700f_") == 0) {
      assert(first.records[i].status == f700f::SweepRunStatus::Completed);
      assert(first.records[i].error_summary.find("surrogate_completed") !=
             std::string::npos);
      assert(first.records[i].implementation_status == "surrogate");
      assert(first.records[i].not_real_modem);
      assert(!first.records[i].downselect_valid);
      assert(first.records[i].not_downselect_valid);
      assert(!first.records[i].performance_valid);
      assert(!first.records[i].surrogate_model_name.empty());
      assert(!first.records[i].surrogate_model_version.empty());
      assert(first.records[i].surrogate_limitations.find("not a real modem") !=
             std::string::npos);
      assert(first.records[i].synthetic_metrics_label.find("synthetic") !=
             std::string::npos);
      ++surrogate_completed;
    }
    if (first.records[i].mode_id == "freedv700d_emulated" ||
        first.records[i].mode_id == "freedv700e_emulated") {
      assert(first.records[i].status == f700f::SweepRunStatus::Completed);
      assert(first.records[i].simulation.ok);
      assert(first.records[i].simulation.stage_status(f700f::PipelineStage::Encode).ok);
      assert(first.records[i].simulation.stage_status(f700f::PipelineStage::Decode).ok);
      assert(first.records[i].simulation.stage_status(f700f::PipelineStage::Metrics).ok);
      assert(first.records[i].error_summary.find("emulated_surrogate_completed") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("implementation_status=emulated_surrogate") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("official=false") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("not_official_freedv=true") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("downselect_valid=false") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("performance_valid=false") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("emulator_model_name=") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("emulator_model_version=") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("emulator_limitations=") !=
             std::string::npos);
      assert(first.records[i].error_summary.find("descriptor_only") ==
             std::string::npos);
      ++emulated_surrogate_completed;
    }
    if (first.records[i].mode_id == "freedv700d_official" ||
        first.records[i].mode_id == "freedv700e_official") {
      if (first.records[i].status == f700f::SweepRunStatus::Skipped) {
        assert(first.records[i].skipped_reason.find(
                   "official_freedv_codec2_unavailable") !=
               std::string::npos);
        ++official_skipped;
      } else {
        assert(first.records[i].status == f700f::SweepRunStatus::Completed);
        assert(first.records[i].simulation.ok);
        assert(first.records[i].simulation.stage_status(f700f::PipelineStage::Encode).ok);
        assert(first.records[i].simulation.stage_status(f700f::PipelineStage::Decode).ok);
        assert(first.records[i].simulation.stage_status(f700f::PipelineStage::Metrics).ok);
        assert(first.records[i].error_summary.find("official_freedv_completed") !=
               std::string::npos);
        assert(first.records[i].error_summary.find("official=true") !=
               std::string::npos);
        assert(first.records[i].error_summary.find("codec2_available=true") !=
               std::string::npos);
        assert(first.records[i].error_summary.find("roundtrip_available=true") !=
               std::string::npos);
        assert(first.records[i].error_summary.find("not_emulator=true") !=
               std::string::npos);
        assert(first.records[i].error_summary.find("not_surrogate=true") !=
               std::string::npos);
        ++official_completed;
      }
    }
    if (first.records[i].mode_id == "unknown.mode.for-skip-test") {
      assert(first.records[i].status == f700f::SweepRunStatus::Skipped);
      assert(first.records[i].skipped_reason ==
             "mode id not registered: unknown.mode.for-skip-test");
      ++unknown_skipped;
    }
  }
  assert(completed > 0);
  assert(skipped > 0);
  assert(failed == 0);
  assert(surrogate_completed == 9);
  assert(emulated_surrogate_completed == 6);
  assert((official_skipped == 6 && official_completed == 0) ||
         (official_skipped == 0 && official_completed == 6));
  assert(unknown_skipped == 3);

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
  assert(json_text.find("surrogate_completed") != std::string::npos);
  assert(json_text.find("\"implementation_status\": \"surrogate\"") !=
         std::string::npos);
  assert(json_text.find("\"not_real_modem\": true") != std::string::npos);
  assert(json_text.find("\"downselect_valid\": false") != std::string::npos);
  assert(json_text.find("\"not_downselect_valid\": true") != std::string::npos);
  assert(json_text.find("\"performance_valid\": false") != std::string::npos);
  assert(json_text.find("surrogate_model_name") != std::string::npos);
  assert(json_text.find("surrogate_model_version") != std::string::npos);
  assert(json_text.find("surrogate_limitations") != std::string::npos);
  assert(json_text.find("surrogate_readiness_score_synthetic") !=
         std::string::npos);
  assert(json_text.find("emulated_surrogate_completed") != std::string::npos);
  assert(json_text.find("not_official_freedv=true") != std::string::npos);
  assert(json_text.find("performance_valid=false") != std::string::npos);
  assert(json_text.find("emulator_limitations=") != std::string::npos);
  assert(json_text.find("descriptor_only_completed") == std::string::npos);
  assert(json_text.find("official_freedv_codec2_unavailable") !=
             std::string::npos ||
         json_text.find("freedv700d_official") != std::string::npos);
  assert(json_text.find("unknown.mode.for-skip-test") != std::string::npos);
  assert(json_text.find("mode id not registered: unknown.mode.for-skip-test") !=
         std::string::npos);

  std::ifstream csv("build/test-artifacts/m2-700f-candidate-smoke/"
                    "m2-700f-candidate-smoke.csv");
  const std::string csv_text((std::istreambuf_iterator<char>(csv)),
                             std::istreambuf_iterator<char>());
  assert(csv_text.find("surrogate_completed") != std::string::npos);
  assert(csv_text.find("not_real_modem") != std::string::npos);
  assert(csv_text.find("downselect_valid") != std::string::npos);
  assert(csv_text.find("not_downselect_valid") != std::string::npos);
  assert(csv_text.find("performance_valid") != std::string::npos);
  assert(csv_text.find("surrogate_readiness_score_synthetic") !=
         std::string::npos);
  assert(csv_text.find("emulated_surrogate_completed") != std::string::npos);
  assert(csv_text.find("not_official_freedv=true") != std::string::npos);
  assert(csv_text.find("performance_valid=false") != std::string::npos);
}

void m2_emulated_surrogate_unknown_channel_fails_clearly() {
  auto runner = make_runner();
  f700f::register_m2_campaign_mode_factories(runner);
  auto config = f700f::make_m2_700f_candidate_smoke_sweep_config(
      "build/test-artifacts/m2-emulated-surrogate-unknown-channel");
  config.run_id_prefix = "m2-emulated-surrogate-unknown-channel";
  config.modes = {{.mode_id = "freedv700d_emulated"}};
  config.channel_conditions = {
      {.condition_id = "unknown-channel",
       .channel_chain = {{.channel_id = "unknown.channel"}}}};
  config.seeds = {1};

  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 1);
  assert(result.records[0].status == f700f::SweepRunStatus::Failed);
  assert(result.records[0].audio_export_path == "N/A");
  assert(result.records[0].error_summary.find(
             "channel id not registered: unknown.channel") != std::string::npos);
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

void m2_smoke_ssb_rows_expose_audio_export_path() {
  auto runner = make_runner();
  f700f::register_m2_campaign_mode_factories(runner);
  auto config = f700f::make_m2_700f_candidate_smoke_sweep_config(
      "build/test-artifacts/m2-700f-candidate-smoke-audio-path");
  config.run_id_prefix = "m2-700f-candidate-smoke-audio-path";
  config.modes = {{.mode_id = "ssb_standard_3k"}, {.mode_id = "ssb_narrow_1k9"}};
  config.export_audio = true;

  const auto result = runner.run(config);
  assert(result.ok);
  assert(std::filesystem::exists(
      "build/test-artifacts/m2-700f-candidate-smoke-audio-path/"
      "m2-700f-candidate-smoke-audio-path.json"));
  assert(std::filesystem::exists(
      "build/test-artifacts/m2-700f-candidate-smoke-audio-path/"
      "m2-700f-candidate-smoke-audio-path.csv"));

  bool saw_standard = false;
  bool saw_narrow = false;
  for (const auto &record : result.records) {
    if (record.mode_id == "ssb_standard_3k" || record.mode_id == "ssb_narrow_1k9") {
      const auto expected_path =
          std::filesystem::path("build/test-artifacts/m2-700f-candidate-smoke-audio-path") /
          "runs" / (record.run_id + ".decoded.raw");
      assert(record.audio_export_path == expected_path.generic_string());
      assert(record.status == f700f::SweepRunStatus::Completed);
      if (record.mode_id == "ssb_standard_3k") {
        saw_standard = true;
      } else {
        saw_narrow = true;
      }
    } else {
      assert(record.audio_export_path == "N/A");
    }
  }
  assert(saw_standard);
  assert(saw_narrow);

  std::ifstream csv_file(
      "build/test-artifacts/m2-700f-candidate-smoke-audio-path/"
      "m2-700f-candidate-smoke-audio-path.csv");
  const std::string csv_text((std::istreambuf_iterator<char>(csv_file)),
                             std::istreambuf_iterator<char>());
  assert(csv_text.find("audio_export_path") != std::string::npos);

  std::ifstream json_file(
      "build/test-artifacts/m2-700f-candidate-smoke-audio-path/"
      "m2-700f-candidate-smoke-audio-path.json");
  const std::string json_text((std::istreambuf_iterator<char>(json_file)),
                              std::istreambuf_iterator<char>());
  assert(json_text.find("audio_export_path") != std::string::npos);
}

void m2_smoke_ssb_rows_disabled_export_are_na() {
  auto runner = make_runner();
  f700f::register_m2_campaign_mode_factories(runner);
  auto config = f700f::make_m2_700f_candidate_smoke_sweep_config(
      "build/test-artifacts/m2-700f-candidate-smoke-audio-disabled");
  config.run_id_prefix = "m2-700f-candidate-smoke-audio-disabled";
  config.modes = {{.mode_id = "ssb_standard_3k"}, {.mode_id = "ssb_narrow_1k9"}};
  config.export_audio = false;

  const auto result = runner.run(config);
  assert(result.ok);
  for (const auto &record : result.records) {
    assert(record.audio_export_path == "N/A");
  }
}

void sweep_invalid_output_directory_fails_clearly() {
  auto runner = make_runner();
  auto config = make_sweep_config();
  const auto invalid_directory = std::string{"build/test-artifacts/invalid-output-dir"};
  {
    std::ofstream invalid_directory_file(invalid_directory);
    invalid_directory_file << "occupied by file";
  }
  config.output_directory = invalid_directory;
  const auto result = runner.run(config);
  assert(!result.ok);
  assert(!result.error.empty());
  assert(result.error.find("failed to create sweep output directory") != std::string::npos);
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
  m2_emulated_surrogate_unknown_channel_fails_clearly();
  m2_700f_candidate_full_campaign_uses_full_matrix();
  m2_smoke_ssb_rows_expose_audio_export_path();
  m2_smoke_ssb_rows_disabled_export_are_na();
  sweep_invalid_output_directory_fails_clearly();
  return 0;
}

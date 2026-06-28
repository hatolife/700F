#include <f700f/sweep_runner.hpp>

#include <cassert>
#include <filesystem>
#include <string>
#include <vector>

namespace {

std::string repo_path(const std::string &path) {
  if (std::filesystem::exists(path)) {
    return path;
  }
  return "../" + path;
}

void smoke_config_loads_from_toml_shape() {
  std::string error;
  auto config = f700f::load_sweep_config_from_file(
      repo_path("configs/sweeps/m2_700f_candidate_smoke.toml"), error);

  assert(error.empty());
  assert(config.run_id_prefix == "m2-700f-candidate-smoke");
  assert(config.output_directory == "reports/sweeps/m2-700f-candidate-smoke");
  assert(config.metric_ids == std::vector<std::string>{"dummy.metric"});
  assert(config.seeds == std::vector<f700f::Seed>{1});
  assert(config.modes.size() == 9);
  assert(config.modes.front().mode_id == "ssb_standard_3k");
  assert(config.modes.back().mode_id == "freedv700f_c_quality");
  assert(config.channel_conditions.size() == 3);
  assert(config.channel_conditions[0].condition_id == "identity");
  assert(config.channel_conditions[1].channel_chain[0].channel_id == "awgn");
  assert(config.channel_conditions[1].channel_chain[0].parameters[0].key ==
         "snr_db");
  assert(config.channel_conditions[1].channel_chain[0].parameters[0].value ==
         "6.0");
}

void missing_config_returns_clear_error() {
  std::string error;
  const auto config = f700f::load_sweep_config_from_file(
      "build/test-artifacts/does-not-exist.toml", error);

  assert(config.modes.empty());
  assert(error.find("missing config") != std::string::npos);
  assert(error.find("build/test-artifacts/does-not-exist.toml") !=
         std::string::npos);
}

void overrides_apply_after_load() {
  f700f::SweepConfigOverrides overrides;
  overrides.output_directory = "build/test-artifacts/cli-override";
  overrides.run_id_prefix = "cli-override-run";

  std::string error;
  auto config = f700f::load_sweep_config_from_file(
      repo_path("configs/sweeps/m2_700f_candidate_smoke.toml"), overrides,
      error);

  assert(error.empty());
  assert(config.output_directory == "build/test-artifacts/cli-override");
  assert(config.run_id_prefix == "cli-override-run");
  assert(config.modes.size() == 9);
}

} // namespace

int main() {
  smoke_config_loads_from_toml_shape();
  missing_config_returns_clear_error();
  overrides_apply_after_load();
  return 0;
}

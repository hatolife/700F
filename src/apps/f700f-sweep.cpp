#include <f700f/sweep_runner.hpp>

#include <iostream>
#include <string>

namespace {

struct CliOptions {
  bool help = false;
  std::string config_path;
  f700f::SweepConfigOverrides overrides;
};

void print_help(std::ostream &out) {
  out << "Usage: f700f-sweep --config <path> [--output-dir <path>] "
         "[--run-id <id>]\n"
      << "\n"
      << "Runs a sweep config and writes aggregate JSON/CSV artifacts.\n"
      << "\n"
      << "Options:\n"
      << "  --help              Show this help text.\n"
      << "  --config <path>     Sweep TOML config to load.\n"
      << "  --output-dir <path> Override output_directory from the config.\n"
      << "  --run-id <id>       Override run_id_prefix from the config.\n";
}

bool parse_args(int argc, char **argv, CliOptions &options, std::string &error) {
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    const auto require_value = [&](const std::string &name) -> std::string {
      if (i + 1 >= argc) {
        error = name + " requires a value";
        return {};
      }
      return argv[++i];
    };

    if (arg == "--help" || arg == "-h") {
      options.help = true;
    } else if (arg == "--config") {
      options.config_path = require_value(arg);
      if (!error.empty()) {
        return false;
      }
    } else if (arg == "--output-dir") {
      options.overrides.output_directory = require_value(arg);
      if (!error.empty()) {
        return false;
      }
    } else if (arg == "--run-id") {
      options.overrides.run_id_prefix = require_value(arg);
      if (!error.empty()) {
        return false;
      }
    } else {
      error = "unknown option: " + arg;
      return false;
    }
  }
  return true;
}

f700f::SweepRunner make_default_runner() {
  f700f::SweepRunner runner;
  f700f::register_m2_campaign_mode_factories(runner);
  runner.register_channel_factory(f700f::make_identity_channel_factory());
  runner.register_channel_factory(f700f::make_awgn_channel_factory());
  runner.register_channel_factory(f700f::make_frequency_offset_channel_factory());
  runner.register_channel_factory(f700f::make_simple_gain_fading_channel_factory());
  runner.register_metric(f700f::make_dummy_metric());
  return runner;
}

} // namespace

int main(int argc, char **argv) {
  CliOptions options;
  std::string error;
  if (!parse_args(argc, argv, options, error)) {
    std::cerr << "f700f-sweep: " << error << '\n';
    return 2;
  }

  if (options.help) {
    print_help(std::cout);
    return 0;
  }

  if (options.config_path.empty()) {
    std::cerr << "f700f-sweep: missing required --config <path>\n";
    return 2;
  }

  auto config = f700f::load_sweep_config_from_file(
      options.config_path, options.overrides, error);
  if (!error.empty()) {
    std::cerr << "f700f-sweep: " << error << '\n';
    return 1;
  }

  auto runner = make_default_runner();
  const auto result = runner.run(config);
  if (!result.ok) {
    std::cerr << "f700f-sweep: " << result.error << '\n';
    return 1;
  }

  std::size_t completed = 0;
  std::size_t skipped = 0;
  std::size_t failed = 0;
  for (const auto &record : result.records) {
    switch (record.status) {
    case f700f::SweepRunStatus::Completed:
      ++completed;
      break;
    case f700f::SweepRunStatus::Skipped:
      ++skipped;
      break;
    case f700f::SweepRunStatus::Failed:
      ++failed;
      break;
    }
  }

  std::cout << "f700f-sweep: completed " << completed << ", skipped "
            << skipped << ", failed " << failed << '\n';
  for (const auto &artifact : result.artifacts) {
    std::cout << artifact.path << '\n';
  }

  return failed == 0 ? 0 : 1;
}

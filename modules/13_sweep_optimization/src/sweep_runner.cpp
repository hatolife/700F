#include <f700f/sweep_runner.hpp>

#include <f700f/candidate_profiles.hpp>
#include <f700f/channel_model.hpp>
#include <f700f/reference_baselines/freedv_emulator.hpp>

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace f700f {
namespace {

std::string escape_json(const std::string &value) {
  std::string out;
  out.reserve(value.size() + 8);
  for (const char c : value) {
    switch (c) {
    case '"':
      out += "\\\"";
      break;
    case '\\':
      out += "\\\\";
      break;
    case '\n':
      out += "\\n";
      break;
    case '\r':
      out += "\\r";
      break;
    case '\t':
      out += "\\t";
      break;
    default:
      out.push_back(c);
      break;
    }
  }
  return out;
}

std::string csv_quote(const std::string &value) {
  if (value.find_first_of(",\"\n\r") == std::string::npos) {
    return value;
  }
  std::string out = "\"";
  for (const char c : value) {
    if (c == '"') {
      out.push_back('"');
    }
    out.push_back(c);
  }
  out.push_back('"');
  return out;
}

std::string sanitize_token(const std::string &value) {
  std::string out;
  out.reserve(value.size());
  for (const char c : value) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9')) {
      out.push_back(c);
    } else {
      out.push_back('-');
    }
  }
  return out.empty() ? "item" : out;
}

std::string make_run_id(const SweepConfig &config,
                        std::size_t index,
                        const ModeId &mode_id,
                        const std::string &condition_id,
                        Seed seed) {
  std::ostringstream out;
  out << (config.run_id_prefix.empty() ? "sweep" : config.run_id_prefix)
      << '-' << std::setw(3) << std::setfill('0') << index << '-'
      << sanitize_token(mode_id) << '-' << sanitize_token(condition_id)
      << "-seed-" << seed;
  return out.str();
}

std::string get_parameter(const std::vector<Capability> &parameters,
                          const std::string &key,
                          const std::string &fallback) {
  const auto it = std::find_if(parameters.begin(), parameters.end(),
                               [&key](const Capability &parameter) {
                                 return parameter.key == key;
                               });
  return it == parameters.end() ? fallback : it->value;
}

double get_double_parameter(const std::vector<Capability> &parameters,
                            const std::string &key,
                            double fallback) {
  const auto value = get_parameter(parameters, key, {});
  if (value.empty()) {
    return fallback;
  }
  try {
    return std::stod(value);
  } catch (const std::exception &) {
    return fallback;
  }
}

bool parse_double_parameter(const std::vector<Capability> &parameters,
                            const std::string &key,
                            double fallback,
                            double &out,
                            std::string &error) {
  const auto value = get_parameter(parameters, key, {});
  if (value.empty()) {
    out = fallback;
    return true;
  }

  char *end = nullptr;
  errno = 0;
  const auto parsed = std::strtod(value.c_str(), &end);
  if (end == value.c_str() || *end != '\0' || errno == ERANGE ||
      !std::isfinite(parsed)) {
    error = "invalid " + key + " parameter: " + value;
    return false;
  }

  out = parsed;
  return true;
}

bool write_text_file(const std::filesystem::path &path,
                     const std::string &contents,
                     std::string &error) {
  std::error_code ec;
  std::filesystem::create_directories(path.parent_path(), ec);
  if (ec) {
    error = "failed to create sweep output directory: " + ec.message();
    return false;
  }
  std::ofstream out(path);
  if (!out) {
    error = "failed to open sweep artifact: " + path.string();
    return false;
  }
  out << contents;
  return true;
}

SimulationConfig make_simulation_config(const SweepConfig &config,
                                        const SweepRunRecord &record,
                                        const SweepChannelCondition &condition) {
  SimulationConfig simulation;
  simulation.run_id = record.run_id;
  simulation.seed = record.seed;
  simulation.input = config.input;
  simulation.mode_id = record.mode_id;
  simulation.channel_chain = condition.channel_chain;
  simulation.output_directory = config.output_directory.empty()
                                    ? std::string{}
                                    : (std::filesystem::path(config.output_directory) /
                                       "runs")
                                          .generic_string();
  simulation.export_audio = config.export_audio;
  simulation.metric_ids = config.metric_ids;
  return simulation;
}

bool contains_token(const std::string &value, const std::string &token) {
  return value.find(token) != std::string::npos;
}

bool is_metadata_only_mode(const ModeDescriptor &descriptor) {
  return contains_token(descriptor.implementation_status, "profile_only") ||
         contains_token(descriptor.implementation_status, "descriptor-only");
}

std::string metadata_only_note(const ModeDescriptor &descriptor) {
  if (contains_token(descriptor.implementation_status, "profile_only")) {
    return "profile_only_completed: waveform encode/decode not run";
  }
  return "descriptor_only_completed: waveform encode/decode not run";
}

bool is_official_freedv_mode(const ModeId &mode_id) {
  return mode_id == "freedv700d_official" ||
         mode_id == "freedv700e_official";
}

std::string unavailable_mode_reason(const ModeId &mode_id) {
  if (is_official_freedv_mode(mode_id)) {
    return "official_waveform_roundtrip_not_implemented: default M2 smoke "
           "keeps official FreeDV skipped until Codec2 official roundtrip is "
           "wired";
  }
  return "mode id not registered: " + mode_id;
}

std::string hex_digest(std::uint64_t value) {
  std::ostringstream out;
  out << std::hex << std::setw(16) << std::setfill('0') << value;
  return out.str();
}

std::string metadata_only_digest(const SimulationConfig &config,
                                 const std::string &note) {
  constexpr std::uint64_t offset = 14695981039346656037ULL;
  constexpr std::uint64_t prime = 1099511628211ULL;
  std::uint64_t hash = offset;
  const auto mix = [&](const std::string &value) {
    for (const unsigned char c : value) {
      hash ^= c;
      hash *= prime;
    }
  };
  mix(config.run_id);
  mix(config.mode_id);
  mix(std::to_string(config.seed));
  mix(note);
  for (const auto &channel : config.channel_chain) {
    mix(channel.channel_id);
  }
  return hex_digest(hash);
}

SimulationResult make_metadata_only_simulation_result(
    const SimulationConfig &config, const ModeDescriptor &descriptor) {
  const auto note = metadata_only_note(descriptor);
  SimulationResult result;
  result.ok = true;
  result.run_id = config.run_id;
  result.seed = config.seed;
  result.mode_id = config.mode_id;
  for (const auto &channel : config.channel_chain) {
    result.channel_ids.push_back(channel.channel_id);
  }
  result.deterministic_digest = metadata_only_digest(config, note);
  result.stage_statuses = {
      {PipelineStage::Input, true, note + "; input config recorded only"},
      {PipelineStage::Encode, true, note + "; encode not run"},
      {PipelineStage::Channel, true, note + "; channel chain recorded only"},
      {PipelineStage::Decode, true, note + "; decode not run"},
      {PipelineStage::Metrics, true, note + "; metrics not evaluated"}};
  return result;
}

std::vector<SweepModeConfig> make_m2_700f_candidate_modes() {
  return {{.mode_id = "ssb_standard_3k"},
          {.mode_id = "ssb_narrow_1k9"},
          {.mode_id = "freedv700d_emulated"},
          {.mode_id = "freedv700e_emulated"},
          {.mode_id = "freedv700d_official"},
          {.mode_id = "freedv700e_official"},
          {.mode_id = "freedv700f_a_balanced"},
          {.mode_id = "freedv700f_b_robust"},
          {.mode_id = "freedv700f_c_quality"}};
}

class EffectChannel final : public IChannel {
public:
  using Maker = std::unique_ptr<ChannelEffect> (*)(const ChannelConfig &, Seed,
                                                   std::string &);

  EffectChannel(std::string channel_id, Maker maker)
      : channel_id_(std::move(channel_id)), maker_(maker) {}

  std::string channel_id() const override { return channel_id_; }

  bool configure(const ChannelConfig &config, Seed seed, std::string &error) override {
    if (config.channel_id != channel_id_) {
      error = channel_id_ + " channel received config for " + config.channel_id;
      return false;
    }
    effect_ = maker_(config, seed, error);
    if (!effect_) {
      if (error.empty()) {
        error = "failed to configure channel: " + channel_id_;
      }
      return false;
    }
    return true;
  }

  ComplexBlock apply(const ComplexBlock &input, std::string &error) override {
    auto output = input;
    if (!effect_->apply(output)) {
      error = std::string(effect_->last_error());
    }
    return output;
  }

private:
  std::string channel_id_;
  Maker maker_;
  std::unique_ptr<ChannelEffect> effect_;
};

class EffectChannelFactory final : public IChannelFactory {
public:
  EffectChannelFactory(std::string channel_id, EffectChannel::Maker maker)
      : channel_id_(std::move(channel_id)), maker_(maker) {}

  std::string channel_id() const override { return channel_id_; }

  std::unique_ptr<IChannel> create() const override {
    return std::make_unique<EffectChannel>(channel_id_, maker_);
  }

private:
  std::string channel_id_;
  EffectChannel::Maker maker_;
};

std::unique_ptr<ChannelEffect> make_awgn_effect(const ChannelConfig &config,
                                                Seed seed,
                                                std::string &error) {
  double snr_db = 24.0;
  if (!parse_double_parameter(config.parameters, "snr_db", snr_db, snr_db,
                              error)) {
    return nullptr;
  }
  return std::make_unique<AwgnChannel>(snr_db, seed);
}

std::unique_ptr<ChannelEffect> make_frequency_offset_effect(
    const ChannelConfig &config, Seed, std::string &error) {
  double freq_offset_hz = 0.0;
  if (!parse_double_parameter(config.parameters, "freq_offset_hz",
                              freq_offset_hz, freq_offset_hz, error)) {
    return nullptr;
  }
  if (freq_offset_hz < 0.0) {
    error = "invalid freq_offset_hz parameter: must be non-negative";
    return nullptr;
  }
  return std::make_unique<FrequencyOffsetChannel>(freq_offset_hz);
}

std::unique_ptr<ChannelEffect> make_simple_gain_fading_effect(
    const ChannelConfig &config, Seed seed, std::string &error) {
  double min_gain_db = -2.0;
  double max_gain_db = 2.0;
  if (!parse_double_parameter(config.parameters, "min_gain_db", min_gain_db,
                              min_gain_db, error) ||
      !parse_double_parameter(config.parameters, "max_gain_db", max_gain_db,
                              max_gain_db, error)) {
    return nullptr;
  }
  if (min_gain_db > max_gain_db) {
    error = "invalid simple_gain_fading parameters: min_gain_db exceeds max_gain_db";
    return nullptr;
  }
  return std::make_unique<SimpleGainFadingChannel>(min_gain_db, max_gain_db,
                                                   seed);
}

void emit_aggregate_artifacts(const SweepConfig &config, SweepResult &result) {
  if (config.output_directory.empty()) {
    return;
  }

  const auto base = std::filesystem::path(config.output_directory);
  const auto prefix = config.run_id_prefix.empty() ? std::string{"sweep"}
                                                  : config.run_id_prefix;
  std::string error;

  const auto json_path = base / (prefix + ".json");
  if (!write_text_file(json_path, sweep_result_to_json(result), error)) {
    result.ok = false;
    result.error = error;
    return;
  }
  result.artifacts.push_back(
      {"sweep-summary-json", json_path.generic_string(), "application/json"});

  const auto csv_path = base / (prefix + ".csv");
  if (!write_text_file(csv_path, sweep_result_to_csv(result), error)) {
    result.ok = false;
    result.error = error;
    return;
  }
  result.artifacts.push_back(
      {"sweep-summary-csv", csv_path.generic_string(), "text/csv"});
}

} // namespace

bool SweepRunner::register_mode_factory(std::shared_ptr<IModeFactory> factory) {
  if (!factory || factory->descriptor().mode_id.empty()) {
    return false;
  }
  const auto descriptor = factory->descriptor();
  const auto mode_id = descriptor.mode_id;
  if (!simulation_runner_.register_mode_factory(std::move(factory))) {
    return false;
  }
  available_modes_.insert(mode_id);
  available_mode_descriptors_.emplace(mode_id, descriptor);
  return true;
}

bool SweepRunner::register_channel_factory(
    std::shared_ptr<IChannelFactory> factory) {
  return simulation_runner_.register_channel_factory(std::move(factory));
}

bool SweepRunner::register_metric(std::shared_ptr<IMetric> metric) {
  return simulation_runner_.register_metric(std::move(metric));
}

SweepResult SweepRunner::run(const SweepConfig &config) const {
  SweepResult result;
  result.run_id_prefix = config.run_id_prefix;

  if (config.modes.empty()) {
    result.error = "sweep config must include at least one mode";
    return result;
  }
  if (config.channel_conditions.empty()) {
    result.error = "sweep config must include at least one channel condition";
    return result;
  }
  if (config.seeds.empty()) {
    result.error = "sweep config must include at least one seed";
    return result;
  }
  std::unordered_set<std::string> condition_ids;
  for (const auto &condition : config.channel_conditions) {
    if (!condition_ids.insert(condition.condition_id).second) {
      result.error = "duplicate channel condition id: " + condition.condition_id;
      return result;
    }
  }

  std::size_t index = 0;
  for (const auto &mode : config.modes) {
    for (const auto &condition : config.channel_conditions) {
      for (const auto seed : config.seeds) {
        SweepRunRecord record;
        record.mode_id = mode.mode_id;
        record.condition_id = condition.condition_id;
        record.seed = seed;
        record.run_id =
            make_run_id(config, index++, mode.mode_id, condition.condition_id, seed);

        const auto descriptor_it =
            available_mode_descriptors_.find(mode.mode_id);
        if (available_modes_.find(mode.mode_id) == available_modes_.end() ||
            descriptor_it == available_mode_descriptors_.end()) {
          record.status = mode.skip_if_unavailable ? SweepRunStatus::Skipped
                                                   : SweepRunStatus::Failed;
          record.skipped_reason = unavailable_mode_reason(mode.mode_id);
          record.error_summary = mode.skip_if_unavailable ? std::string{}
                                                          : record.skipped_reason;
          result.records.push_back(std::move(record));
          continue;
        }

        const auto simulation_config =
            make_simulation_config(config, record, condition);
        if (is_metadata_only_mode(descriptor_it->second)) {
          record.simulation = make_metadata_only_simulation_result(
              simulation_config, descriptor_it->second);
          record.status = SweepRunStatus::Completed;
          record.error_summary = metadata_only_note(descriptor_it->second);
          result.records.push_back(std::move(record));
          continue;
        }

        record.simulation = simulation_runner_.run(simulation_config);
        if (record.simulation.ok) {
          record.status = SweepRunStatus::Completed;
        } else {
          record.status = SweepRunStatus::Failed;
          record.error_summary = record.simulation.error;
        }
        result.records.push_back(std::move(record));
      }
    }
  }

  result.ok = true;
  emit_aggregate_artifacts(config, result);
  return result;
}

std::string sweep_run_status_name(SweepRunStatus status) {
  switch (status) {
  case SweepRunStatus::Completed:
    return "completed";
  case SweepRunStatus::Skipped:
    return "skipped";
  case SweepRunStatus::Failed:
    return "failed";
  }
  return "unknown";
}

std::string sweep_result_to_json(const SweepResult &result) {
  std::ostringstream out;
  out << "{\n";
  out << "  \"ok\": " << (result.ok ? "true" : "false") << ",\n";
  out << "  \"run_id_prefix\": \"" << escape_json(result.run_id_prefix)
      << "\",\n";
  out << "  \"error\": \"" << escape_json(result.error) << "\",\n";
  out << "  \"records\": [\n";
  for (std::size_t i = 0; i < result.records.size(); ++i) {
    const auto &record = result.records[i];
    out << "    {\"run_id\": \"" << escape_json(record.run_id)
        << "\", \"status\": \"" << sweep_run_status_name(record.status)
        << "\", \"mode_id\": \"" << escape_json(record.mode_id)
        << "\", \"condition_id\": \"" << escape_json(record.condition_id)
        << "\", \"seed\": " << record.seed << ", \"skipped_reason\": ";
    if (record.skipped_reason.empty()) {
      out << "null";
    } else {
      out << '"' << escape_json(record.skipped_reason) << '"';
    }
    out << ", \"error_summary\": ";
    if (record.error_summary.empty()) {
      out << "null";
    } else {
      out << '"' << escape_json(record.error_summary) << '"';
    }
    out << ", \"simulation_digest\": \""
        << escape_json(record.simulation.deterministic_digest) << "\"}";
    out << (i + 1 == result.records.size() ? "\n" : ",\n");
  }
  out << "  ]\n";
  out << "}\n";
  return out.str();
}

std::string sweep_result_to_csv(const SweepResult &result) {
  std::ostringstream out;
  out << "run_id,status,mode_id,condition_id,seed,simulation_ok,digest,"
         "skipped_reason,error_summary\n";
  for (const auto &record : result.records) {
    out << csv_quote(record.run_id) << ','
        << sweep_run_status_name(record.status) << ','
        << csv_quote(record.mode_id) << ','
        << csv_quote(record.condition_id) << ',' << record.seed << ','
        << (record.simulation.ok ? "true" : "false") << ','
        << csv_quote(record.simulation.deterministic_digest) << ','
        << csv_quote(record.skipped_reason) << ','
        << csv_quote(record.error_summary) << '\n';
  }
  return out.str();
}

std::shared_ptr<IChannelFactory> make_awgn_channel_factory() {
  return std::make_shared<EffectChannelFactory>("awgn", make_awgn_effect);
}

std::shared_ptr<IChannelFactory> make_frequency_offset_channel_factory() {
  return std::make_shared<EffectChannelFactory>("frequency_offset",
                                                make_frequency_offset_effect);
}

std::shared_ptr<IChannelFactory> make_simple_gain_fading_channel_factory() {
  return std::make_shared<EffectChannelFactory>("simple_gain_fading",
                                                make_simple_gain_fading_effect);
}

SweepConfig make_m1_baseline_smoke_sweep_config(std::string output_directory) {
  SweepConfig config;
  config.run_id_prefix = "m1-baseline-smoke";
  config.output_directory = std::move(output_directory);
  config.input = GeneratedToneConfig{.sample_rate_hz = 8000,
                                     .sample_count = 32,
                                     .frequency_hz = 1000.0F,
                                     .amplitude = 0.25F};
  config.metric_ids = {"dummy.metric"};
  config.modes = {{.mode_id = "ssb_standard_3k"},
                  {.mode_id = "ssb_narrow_1k9"},
                  {.mode_id = "freedv700d_emulated"},
                  {.mode_id = "freedv700e_emulated"},
                  {.mode_id = "freedv700d_official"},
                  {.mode_id = "freedv700e_official"}};
  config.channel_conditions = {
      {.condition_id = "identity",
       .channel_chain = {{.channel_id = "identity"}}},
      {.condition_id = "awgn-high",
       .channel_chain = {{.channel_id = "awgn",
                          .parameters = {{"snr_db", "24.0"}}}}},
      {.condition_id = "awgn-low",
       .channel_chain = {{.channel_id = "awgn",
                          .parameters = {{"snr_db", "6.0"}}}}},
      {.condition_id = "frequency-offset",
       .channel_chain = {{.channel_id = "frequency_offset",
                          .parameters = {{"freq_offset_hz", "25.0"}}}}},
      {.condition_id = "simple-fading",
       .channel_chain = {{.channel_id = "simple_gain_fading",
                          .parameters = {{"min_gain_db", "-1.5"},
                                         {"max_gain_db", "1.5"}}}}}};
  config.seeds = {1};
  return config;
}

SweepConfig make_m2_channel_matrix_smoke_sweep_config(
    std::string output_directory) {
  SweepConfig config;
  config.run_id_prefix = "m2-channel-matrix-smoke";
  config.output_directory = std::move(output_directory);
  config.input = GeneratedToneConfig{.sample_rate_hz = 8000,
                                     .sample_count = 32,
                                     .frequency_hz = 1000.0F,
                                     .amplitude = 0.25F};
  config.metric_ids = {"dummy.metric"};
  config.channel_conditions = {
      {.condition_id = "identity",
       .channel_chain = {{.channel_id = "identity"}}},
      {.condition_id = "awgn-snr-6db",
       .channel_chain = {{.channel_id = "awgn",
                          .parameters = {{"snr_db", "6.0"}}}}},
      {.condition_id = "awgn-snr-0db",
       .channel_chain = {{.channel_id = "awgn",
                          .parameters = {{"snr_db", "0.0"}}}}}};
  config.seeds = {1};
  return config;
}

SweepConfig make_m2_channel_matrix_full_sweep_config(
    std::string output_directory) {
  SweepConfig config;
  config.run_id_prefix = "m2-channel-matrix-full";
  config.output_directory = std::move(output_directory);
  config.input = GeneratedToneConfig{.sample_rate_hz = 8000,
                                     .sample_count = 32,
                                     .frequency_hz = 1000.0F,
                                     .amplitude = 0.25F};
  config.metric_ids = {"dummy.metric"};

  constexpr std::array<int, 6> snr_db_values = {-2, 0, 2, 4, 6, 8};
  constexpr std::array<int, 4> frequency_offsets_hz = {0, 50, 100, 200};
  constexpr std::array<const char *, 3> fading_profiles = {"none", "weak",
                                                           "medium"};

  for (const auto snr_db : snr_db_values) {
    for (const auto freq_offset_hz : frequency_offsets_hz) {
      for (const auto *fading : fading_profiles) {
        SweepChannelCondition condition;
        condition.condition_id = "awgn-snr-" + std::to_string(snr_db) +
                                 "db-fo-" +
                                 std::to_string(freq_offset_hz) +
                                 "hz-fading-" + fading;
        condition.channel_chain = {
            {.channel_id = "awgn",
             .parameters = {{"snr_db", std::to_string(snr_db) + ".0"}}},
            {.channel_id = "frequency_offset",
             .parameters = {{"freq_offset_hz",
                             std::to_string(freq_offset_hz) + ".0"}}}};
        if (std::string{fading} == "weak") {
          condition.channel_chain.push_back(
              {.channel_id = "simple_gain_fading",
               .parameters = {{"min_gain_db", "-1.5"},
                              {"max_gain_db", "1.5"}}});
        } else if (std::string{fading} == "medium") {
          condition.channel_chain.push_back(
              {.channel_id = "simple_gain_fading",
               .parameters = {{"min_gain_db", "-3.0"},
                              {"max_gain_db", "3.0"}}});
        }
        config.channel_conditions.push_back(std::move(condition));
      }
    }
  }
  config.seeds = {1, 2, 3};
  return config;
}

SweepConfig make_m2_700f_candidate_smoke_sweep_config(
    std::string output_directory) {
  auto config = make_m2_channel_matrix_smoke_sweep_config(
      std::move(output_directory));
  config.run_id_prefix = "m2-700f-candidate-smoke";
  config.modes = make_m2_700f_candidate_modes();
  return config;
}

SweepConfig make_m2_700f_candidate_full_sweep_config(
    std::string output_directory) {
  auto config = make_m2_channel_matrix_full_sweep_config(
      std::move(output_directory));
  config.run_id_prefix = "m2-700f-candidate-full";
  config.modes = make_m2_700f_candidate_modes();
  return config;
}

void register_m2_campaign_mode_factories(SweepRunner &runner) {
  runner.register_mode_factory(make_ssb_standard_3k_mode_factory());
  runner.register_mode_factory(make_ssb_narrow_1k9_mode_factory());
  runner.register_mode_factory(make_freedv700d_emulated_mode_factory());
  runner.register_mode_factory(make_freedv700e_emulated_mode_factory());
  runner.register_mode_factory(
      make_700f_candidate_profile_factory("freedv700f_a_balanced"));
  runner.register_mode_factory(
      make_700f_candidate_profile_factory("freedv700f_b_robust"));
  runner.register_mode_factory(
      make_700f_candidate_profile_factory("freedv700f_c_quality"));
}

} // namespace f700f

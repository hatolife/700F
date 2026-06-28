#include <f700f/sweep_runner.hpp>

#include <f700f/candidate_profiles.hpp>
#include <f700f/channel_model.hpp>
#include <f700f/codec_adapter/freedv_official.hpp>
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

bool is_surrogate_mode(const ModeDescriptor &descriptor) {
  return descriptor.implementation_status == "surrogate";
}

bool is_waveform_prototype_mode(const ModeDescriptor &descriptor) {
  return descriptor.implementation_status == "waveform_prototype";
}

bool is_real_modem_prototype_mode(const ModeDescriptor &descriptor) {
  return descriptor.implementation_status == "real_modem_prototype";
}

bool is_metadata_only_mode(const ModeDescriptor &descriptor) {
  return is_surrogate_mode(descriptor) ||
         contains_token(descriptor.implementation_status, "profile_only") ||
         contains_token(descriptor.implementation_status, "descriptor-only");
}

bool is_emulated_surrogate_mode(const ModeDescriptor &descriptor) {
  return descriptor.implementation_status == "emulated_surrogate";
}

std::string metadata_only_note(const ModeDescriptor &descriptor) {
  if (is_surrogate_mode(descriptor)) {
    return "surrogate_completed: not_real_modem=true downselect_valid=false "
           "not_downselect_valid=true performance_valid=false "
           "synthetic_metrics_label=synthetic_surrogate_readiness_only";
  }
  if (contains_token(descriptor.implementation_status, "profile_only")) {
    return "profile_only_completed: waveform encode/decode not run";
  }
  return "descriptor_only_completed: waveform encode/decode not run";
}

void apply_surrogate_metadata(SweepRunRecord &record,
                              const ModeDescriptor &descriptor) {
  record.implementation_status = descriptor.implementation_status;
  if (!is_surrogate_mode(descriptor)) {
    return;
  }
  record.not_real_modem = true;
  record.downselect_valid = false;
  record.not_downselect_valid = true;
  record.performance_valid = false;
  record.surrogate_model_name = "700f_candidate_minimal_behavior";
  record.surrogate_model_version = "ISSUE-0032-v1";
  record.surrogate_limitations =
      "synthetic readiness only; not a real modem; BER/FER are not emitted as real values";
  record.surrogate_readiness_score_synthetic = "0.625";
  record.synthetic_metrics_label = "synthetic_surrogate_readiness_only";
}

std::string waveform_prototype_note() {
  return "waveform_prototype_completed: implementation_status=waveform_prototype; "
         "prototype=true; not_final_modem=true; waveform_capable=true; "
         "codec_family=synthetic; fec_family=none; "
         "modem_family=toy_audio_waveform; downselect_valid=false; "
         "performance_valid=false; prototype_limitations=toy audio waveform, "
         "synthetic codec, no FEC, not final modem, not official FreeDV, "
         "not valid for real downselect";
}

void apply_waveform_prototype_metadata(SweepRunRecord &record,
                                       const ModeDescriptor &descriptor) {
  record.implementation_status = descriptor.implementation_status;
  if (!is_waveform_prototype_mode(descriptor)) {
    return;
  }
  record.prototype = true;
  record.not_final_modem = true;
  record.waveform_capable = true;
  record.downselect_valid = false;
  record.not_downselect_valid = true;
  record.performance_valid = false;
  record.codec_family = "synthetic";
  record.fec_family = "none";
  record.modem_family = "toy_audio_waveform";
  record.prototype_limitations =
      "toy audio waveform; synthetic codec; no FEC; not final modem; not official FreeDV; not valid for real downselect";
}

std::string real_modem_prototype_note(
    std::uint64_t prototype_baseband_sample_count) {
  return "real_modem_prototype_completed: "
         "implementation_status=real_modem_prototype; "
         "implementation_classification=real_modem_prototype; "
         "prototype=true; not_final_modem=true; waveform_capable=true; "
         "codec_family=synthetic; fec_family=none; sync_family=none; "
         "modem_family=minimal_qpsk; downselect_valid=false; "
         "performance_valid=false; performance_validity=limited; "
         "performance_valid=limited; downselect_validity=invalid; "
         "prototype_frame_status=completed; prototype_sync_status=none; "
         "prototype_baseband_sample_count=" +
         std::to_string(prototype_baseband_sample_count) +
         "; prototype_limitations=ISSUE-0042 minimal QPSK-like baseband "
         "prototype, synthetic codec, no FEC, no final synchronization, "
         "not final modem, not official FreeDV, not valid for real downselect";
}

void apply_real_modem_prototype_metadata(SweepRunRecord &record,
                                         const ModeDescriptor &descriptor) {
  record.implementation_status = descriptor.implementation_status;
  if (!is_real_modem_prototype_mode(descriptor)) {
    return;
  }
  record.implementation_classification = "real_modem_prototype";
  record.prototype = true;
  record.not_final_modem = true;
  record.waveform_capable = true;
  record.downselect_valid = false;
  record.not_downselect_valid = true;
  record.performance_valid = false;
  record.performance_validity = "limited";
  record.downselect_validity = "invalid";
  record.codec_family = "synthetic";
  record.fec_family = "none";
  record.sync_family = "none";
  record.modem_family = "minimal_qpsk";
  record.prototype_warning =
      "REAL MODEM PROTOTYPE WARNING: limited diagnostics only; "
      "performance_valid=limited; not real performance; "
      "downselect_valid=false";
  record.prototype_symbol_error_rate = "N/A";
  record.prototype_frame_status = "completed";
  record.prototype_sync_status = "none";
  record.prototype_limitations =
      "ISSUE-0042 minimal QPSK-like baseband prototype; synthetic codec; "
      "no FEC; no final synchronization; not final modem; not official FreeDV; "
      "not valid for real downselect";
}

std::string emulated_surrogate_note() {
  return std::string("emulated_surrogate_completed: "
                     "implementation_status=emulated_surrogate; "
                     "official=false; not_official_freedv=true; "
                     "downselect_valid=false; performance_valid=false; "
                     "emulator_model_name=") +
         freedv_emulator_model_name() + "; emulator_model_version=" +
         freedv_emulator_model_version() + "; emulator_limitations=" +
         freedv_emulator_limitations();
}

std::string official_freedv_completed_note() {
  return "official_freedv_completed: official=true; codec2_enabled=true; "
         "codec2_available=true; roundtrip_available=true; "
         "not_emulator=true; not_surrogate=true";
}

bool is_official_freedv_mode(const ModeId &mode_id) {
  return mode_id == "freedv700d_official" ||
         mode_id == "freedv700e_official";
}

std::string unavailable_mode_reason(const ModeId &mode_id) {
  if (is_official_freedv_mode(mode_id)) {
    if (!freedv_official_codec2_available()) {
      return "official_freedv_codec2_unavailable: F700F_ENABLE_CODEC2=OFF; "
             "official=true codec2_enabled=false codec2_available=false "
             "roundtrip_available=false not_emulator=true not_surrogate=true";
    }
    return "official_freedv_not_registered: Codec2 runtime is enabled but "
           "official FreeDV mode factory was not registered";
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

std::string trim_copy(const std::string &value) {
  const auto begin = value.find_first_not_of(" \t\r\n");
  if (begin == std::string::npos) {
    return {};
  }
  const auto end = value.find_last_not_of(" \t\r\n");
  return value.substr(begin, end - begin + 1);
}

std::string strip_comment(const std::string &line) {
  bool in_string = false;
  std::string out;
  for (std::size_t i = 0; i < line.size(); ++i) {
    const char c = line[i];
    if (c == '"' && (i == 0 || line[i - 1] != '\\')) {
      in_string = !in_string;
    }
    if (c == '#' && !in_string) {
      break;
    }
    out.push_back(c);
  }
  return trim_copy(out);
}

bool parse_toml_string(const std::string &value,
                       std::string &out,
                       std::string &error) {
  const auto trimmed = trim_copy(value);
  if (trimmed.size() < 2 || trimmed.front() != '"' ||
      trimmed.back() != '"') {
    error = "expected quoted string";
    return false;
  }

  out.clear();
  for (std::size_t i = 1; i + 1 < trimmed.size(); ++i) {
    if (trimmed[i] == '\\' && i + 2 < trimmed.size()) {
      const char escaped = trimmed[++i];
      switch (escaped) {
      case '"':
      case '\\':
        out.push_back(escaped);
        break;
      case 'n':
        out.push_back('\n');
        break;
      case 'r':
        out.push_back('\r');
        break;
      case 't':
        out.push_back('\t');
        break;
      default:
        error = "unsupported string escape";
        return false;
      }
    } else {
      out.push_back(trimmed[i]);
    }
  }
  return true;
}

std::vector<std::string> split_toml_array_items(const std::string &value,
                                                std::string &error) {
  std::vector<std::string> items;
  const auto trimmed = trim_copy(value);
  if (trimmed.size() < 2 || trimmed.front() != '[' ||
      trimmed.back() != ']') {
    error = "expected array";
    return items;
  }

  bool in_string = false;
  std::string item;
  for (std::size_t i = 1; i + 1 < trimmed.size(); ++i) {
    const char c = trimmed[i];
    if (c == '"' && (i == 1 || trimmed[i - 1] != '\\')) {
      in_string = !in_string;
    }
    if (c == ',' && !in_string) {
      items.push_back(trim_copy(item));
      item.clear();
    } else {
      item.push_back(c);
    }
  }
  if (in_string) {
    error = "unterminated string in array";
    return {};
  }
  if (!trim_copy(item).empty()) {
    items.push_back(trim_copy(item));
  }
  return items;
}

bool parse_string_array(const std::string &value,
                        std::vector<std::string> &out,
                        std::string &error) {
  const auto items = split_toml_array_items(value, error);
  if (!error.empty()) {
    return false;
  }
  out.clear();
  for (const auto &item : items) {
    std::string parsed;
    if (!parse_toml_string(item, parsed, error)) {
      return false;
    }
    out.push_back(std::move(parsed));
  }
  return true;
}

bool parse_seed_array(const std::string &value,
                      std::vector<Seed> &out,
                      std::string &error) {
  const auto items = split_toml_array_items(value, error);
  if (!error.empty()) {
    return false;
  }
  out.clear();
  for (const auto &item : items) {
    try {
      std::size_t consumed = 0;
      const auto parsed = std::stoull(item, &consumed);
      if (consumed != item.size()) {
        error = "invalid seed value: " + item;
        return false;
      }
      out.push_back(static_cast<Seed>(parsed));
    } catch (const std::exception &) {
      error = "invalid seed value: " + item;
      return false;
    }
  }
  return true;
}

bool parse_bool(const std::string &value, bool &out, std::string &error) {
  const auto trimmed = trim_copy(value);
  if (trimmed == "true") {
    out = true;
    return true;
  }
  if (trimmed == "false") {
    out = false;
    return true;
  }
  error = "expected boolean";
  return false;
}

bool parse_uint32(const std::string &value,
                  std::uint32_t &out,
                  std::string &error) {
  const auto trimmed = trim_copy(value);
  try {
    std::size_t consumed = 0;
    const auto parsed = std::stoul(trimmed, &consumed);
    if (consumed != trimmed.size()) {
      error = "expected unsigned integer";
      return false;
    }
    out = static_cast<std::uint32_t>(parsed);
    return true;
  } catch (const std::exception &) {
    error = "expected unsigned integer";
    return false;
  }
}

bool parse_float(const std::string &value, float &out, std::string &error) {
  const auto trimmed = trim_copy(value);
  try {
    std::size_t consumed = 0;
    const auto parsed = std::stof(trimmed, &consumed);
    if (consumed != trimmed.size()) {
      error = "expected float";
      return false;
    }
    out = parsed;
    return true;
  } catch (const std::exception &) {
    error = "expected float";
    return false;
  }
}

std::string parse_parameter_value(const std::string &value,
                                  std::string &error) {
  const auto trimmed = trim_copy(value);
  if (!trimmed.empty() && trimmed.front() == '"') {
    std::string parsed;
    if (!parse_toml_string(trimmed, parsed, error)) {
      return {};
    }
    return parsed;
  }
  return trimmed;
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
          record.audio_export_path = "N/A";
          result.records.push_back(std::move(record));
          continue;
        }

        const auto simulation_config =
            make_simulation_config(config, record, condition);
        apply_surrogate_metadata(record, descriptor_it->second);
        apply_waveform_prototype_metadata(record, descriptor_it->second);
        apply_real_modem_prototype_metadata(record, descriptor_it->second);
        if (is_metadata_only_mode(descriptor_it->second)) {
          record.simulation = make_metadata_only_simulation_result(
              simulation_config, descriptor_it->second);
          record.status = SweepRunStatus::Completed;
          record.error_summary = metadata_only_note(descriptor_it->second);
          record.audio_export_path = "N/A";
          result.records.push_back(std::move(record));
          continue;
        }

        record.simulation = simulation_runner_.run(simulation_config);
        record.audio_export_path = record.simulation.audio_export_path;
        if (record.audio_export_path.empty()) {
          record.audio_export_path = "N/A";
        }
        if (record.simulation.ok) {
          record.status = SweepRunStatus::Completed;
          if (is_real_modem_prototype_mode(descriptor_it->second)) {
            record.prototype_baseband_sample_count =
                record.simulation.channel_output.iq.size();
            record.error_summary = real_modem_prototype_note(
                record.prototype_baseband_sample_count);
          } else if (is_waveform_prototype_mode(descriptor_it->second)) {
            record.error_summary = waveform_prototype_note();
          } else if (is_emulated_surrogate_mode(descriptor_it->second)) {
            record.error_summary = emulated_surrogate_note();
          } else if (is_official_freedv_mode(record.mode_id)) {
            record.error_summary = official_freedv_completed_note();
          }
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
        << escape_json(record.simulation.deterministic_digest)
        << "\", \"audio_export_path\": \""
        << escape_json(record.audio_export_path)
        << "\", \"implementation_status\": \""
        << escape_json(record.implementation_status)
        << "\", \"implementation_classification\": \""
        << escape_json(record.implementation_classification)
        << "\", \"not_real_modem\": "
        << (record.not_real_modem ? "true" : "false")
        << ", \"downselect_valid\": "
        << (record.downselect_valid ? "true" : "false")
        << ", \"not_downselect_valid\": "
        << (record.not_downselect_valid ? "true" : "false")
        << ", \"performance_valid\": "
        << (record.performance_valid ? "true" : "false")
        << ", \"performance_validity\": \""
        << escape_json(record.performance_validity)
        << "\", \"downselect_validity\": \""
        << escape_json(record.downselect_validity)
        << "\", \"prototype\": "
        << (record.prototype ? "true" : "false")
        << ", \"not_final_modem\": "
        << (record.not_final_modem ? "true" : "false")
        << ", \"waveform_capable\": "
        << (record.waveform_capable ? "true" : "false")
        << ", \"codec_family\": \""
        << escape_json(record.codec_family)
        << "\", \"fec_family\": \""
        << escape_json(record.fec_family)
        << "\", \"sync_family\": \""
        << escape_json(record.sync_family)
        << "\", \"modem_family\": \""
        << escape_json(record.modem_family)
        << "\", \"prototype_limitations\": \""
        << escape_json(record.prototype_limitations)
        << "\", \"prototype_warning\": \""
        << escape_json(record.prototype_warning)
        << "\", \"prototype_symbol_error_rate\": ";
    if (record.prototype_symbol_error_rate.empty() ||
        record.prototype_symbol_error_rate == "N/A") {
      out << "\"N/A\"";
    } else {
      out << record.prototype_symbol_error_rate;
    }
    out << ", \"prototype_frame_status\": \""
        << escape_json(record.prototype_frame_status)
        << "\", \"prototype_sync_status\": \""
        << escape_json(record.prototype_sync_status)
        << "\", \"prototype_baseband_sample_count\": "
        << record.prototype_baseband_sample_count
        << ", \"surrogate_model_name\": \""
        << escape_json(record.surrogate_model_name)
        << "\", \"surrogate_model_version\": \""
        << escape_json(record.surrogate_model_version)
        << "\", \"surrogate_limitations\": \""
        << escape_json(record.surrogate_limitations)
        << "\", \"surrogate_readiness_score_synthetic\": \""
        << escape_json(record.surrogate_readiness_score_synthetic)
        << "\", \"synthetic_metrics_label\": \""
        << escape_json(record.synthetic_metrics_label) << "\"}";
    out << (i + 1 == result.records.size() ? "\n" : ",\n");
  }
  out << "  ]\n";
  out << "}\n";
  return out.str();
}

std::string sweep_result_to_csv(const SweepResult &result) {
  std::ostringstream out;
  out << "run_id,status,mode_id,condition_id,seed,simulation_ok,digest,"
         "audio_export_path,skipped_reason,error_summary,implementation_status,"
         "implementation_classification,not_real_modem,downselect_valid,"
         "not_downselect_valid,performance_valid,performance_validity,"
         "downselect_validity,prototype,not_final_modem,waveform_capable,"
         "codec_family,fec_family,sync_family,modem_family,"
         "prototype_limitations,prototype_warning,prototype_symbol_error_rate,"
         "prototype_frame_status,prototype_sync_status,"
         "prototype_baseband_sample_count,"
         "surrogate_model_name,surrogate_model_version,surrogate_limitations,"
         "surrogate_readiness_score_synthetic,synthetic_metrics_label\n";
  for (const auto &record : result.records) {
    out << csv_quote(record.run_id) << ','
        << sweep_run_status_name(record.status) << ','
        << csv_quote(record.mode_id) << ','
        << csv_quote(record.condition_id) << ',' << record.seed << ','
        << (record.simulation.ok ? "true" : "false") << ','
        << csv_quote(record.simulation.deterministic_digest) << ','
        << csv_quote(record.audio_export_path) << ','
        << csv_quote(record.skipped_reason) << ','
        << csv_quote(record.error_summary) << ','
        << csv_quote(record.implementation_status) << ','
        << csv_quote(record.implementation_classification) << ','
        << (record.not_real_modem ? "true" : "false") << ','
        << (record.downselect_valid ? "true" : "false") << ','
        << (record.not_downselect_valid ? "true" : "false") << ','
        << (record.performance_valid ? "true" : "false") << ','
        << csv_quote(record.performance_validity) << ','
        << csv_quote(record.downselect_validity) << ','
        << (record.prototype ? "true" : "false") << ','
        << (record.not_final_modem ? "true" : "false") << ','
        << (record.waveform_capable ? "true" : "false") << ','
        << csv_quote(record.codec_family) << ','
        << csv_quote(record.fec_family) << ','
        << csv_quote(record.sync_family) << ','
        << csv_quote(record.modem_family) << ','
        << csv_quote(record.prototype_limitations) << ','
        << csv_quote(record.prototype_warning) << ','
        << csv_quote(record.prototype_symbol_error_rate) << ','
        << csv_quote(record.prototype_frame_status) << ','
        << csv_quote(record.prototype_sync_status) << ','
        << record.prototype_baseband_sample_count << ','
        << csv_quote(record.surrogate_model_name) << ','
        << csv_quote(record.surrogate_model_version) << ','
        << csv_quote(record.surrogate_limitations) << ','
        << csv_quote(record.surrogate_readiness_score_synthetic) << ','
        << csv_quote(record.synthetic_metrics_label) << '\n';
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

SweepConfig load_sweep_config_from_file(const std::string &path,
                                        const SweepConfigOverrides &overrides,
                                        std::string &error) {
  error.clear();
  SweepConfig config;

  std::ifstream in(path);
  if (!in) {
    error = "missing config: " + path;
    return config;
  }

  enum class Section {
    Root,
    InputGeneratedTone,
    Mode,
    ChannelCondition,
    ChannelChain,
  };

  Section section = Section::Root;
  auto tone = GeneratedToneConfig{};
  config.input = tone;

  std::string line;
  std::size_t line_number = 0;
  while (std::getline(in, line)) {
    ++line_number;
    const auto cleaned = strip_comment(line);
    if (cleaned.empty()) {
      continue;
    }

    if (cleaned.front() == '[') {
      if (cleaned == "[input.generated_tone]") {
        section = Section::InputGeneratedTone;
      } else if (cleaned == "[[modes]]") {
        config.modes.push_back({});
        section = Section::Mode;
      } else if (cleaned == "[[channel_conditions]]") {
        config.channel_conditions.push_back({});
        section = Section::ChannelCondition;
      } else if (cleaned == "[[channel_conditions.channel_chain]]") {
        if (config.channel_conditions.empty()) {
          error = "line " + std::to_string(line_number) +
                  ": channel_chain declared before channel_conditions";
          return {};
        }
        config.channel_conditions.back().channel_chain.push_back({});
        section = Section::ChannelChain;
      } else {
        error = "line " + std::to_string(line_number) +
                ": unsupported table: " + cleaned;
        return {};
      }
      continue;
    }

    const auto equals = cleaned.find('=');
    if (equals == std::string::npos) {
      error = "line " + std::to_string(line_number) + ": expected key = value";
      return {};
    }
    const auto key = trim_copy(cleaned.substr(0, equals));
    const auto value = trim_copy(cleaned.substr(equals + 1));
    std::string parsed;

    const auto fail = [&](const std::string &message) {
      error = "line " + std::to_string(line_number) + ": " + message;
      return SweepConfig{};
    };

    switch (section) {
    case Section::Root:
      if (key == "run_id_prefix") {
        if (!parse_toml_string(value, config.run_id_prefix, error)) {
          return fail(error);
        }
      } else if (key == "output_directory") {
        if (!parse_toml_string(value, config.output_directory, error)) {
          return fail(error);
        }
      } else if (key == "export_audio") {
        if (!parse_bool(value, config.export_audio, error)) {
          return fail(error);
        }
      } else if (key == "metric_ids") {
        if (!parse_string_array(value, config.metric_ids, error)) {
          return fail(error);
        }
      } else if (key == "seeds") {
        if (!parse_seed_array(value, config.seeds, error)) {
          return fail(error);
        }
      } else {
        return fail("unsupported root key: " + key);
      }
      break;

    case Section::InputGeneratedTone: {
      auto *generated = std::get_if<GeneratedToneConfig>(&config.input);
      if (generated == nullptr) {
        config.input = GeneratedToneConfig{};
        generated = std::get_if<GeneratedToneConfig>(&config.input);
      }
      if (key == "sample_rate_hz") {
        if (!parse_uint32(value, generated->sample_rate_hz, error)) {
          return fail(error);
        }
      } else if (key == "sample_count") {
        if (!parse_uint32(value, generated->sample_count, error)) {
          return fail(error);
        }
      } else if (key == "frequency_hz") {
        if (!parse_float(value, generated->frequency_hz, error)) {
          return fail(error);
        }
      } else if (key == "amplitude") {
        if (!parse_float(value, generated->amplitude, error)) {
          return fail(error);
        }
      } else {
        return fail("unsupported generated_tone key: " + key);
      }
      break;
    }

    case Section::Mode:
      if (config.modes.empty()) {
        return fail("mode key declared before [[modes]]");
      }
      if (key == "mode_id") {
        if (!parse_toml_string(value, config.modes.back().mode_id, error)) {
          return fail(error);
        }
      } else if (key == "skip_if_unavailable") {
        if (!parse_bool(value, config.modes.back().skip_if_unavailable,
                        error)) {
          return fail(error);
        }
      } else {
        return fail("unsupported mode key: " + key);
      }
      break;

    case Section::ChannelCondition:
      if (config.channel_conditions.empty()) {
        return fail("condition key declared before [[channel_conditions]]");
      }
      if (key == "condition_id") {
        if (!parse_toml_string(value,
                               config.channel_conditions.back().condition_id,
                               error)) {
          return fail(error);
        }
      } else {
        return fail("unsupported channel condition key: " + key);
      }
      break;

    case Section::ChannelChain:
      if (config.channel_conditions.empty() ||
          config.channel_conditions.back().channel_chain.empty()) {
        return fail("channel_chain key declared before channel_chain table");
      }
      if (key == "channel_id") {
        if (!parse_toml_string(
                value,
                config.channel_conditions.back().channel_chain.back().channel_id,
                error)) {
          return fail(error);
        }
      } else {
        parsed = parse_parameter_value(value, error);
        if (!error.empty()) {
          return fail(error);
        }
        config.channel_conditions.back()
            .channel_chain.back()
            .parameters.push_back({key, parsed});
      }
      break;
    }
  }

  if (overrides.output_directory.has_value()) {
    config.output_directory = *overrides.output_directory;
  }
  if (overrides.run_id_prefix.has_value()) {
    config.run_id_prefix = *overrides.run_id_prefix;
  }
  return config;
}

SweepConfig load_sweep_config_from_file(const std::string &path,
                                        std::string &error) {
  return load_sweep_config_from_file(path, SweepConfigOverrides{}, error);
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
  if (freedv_official_codec2_available()) {
    runner.register_mode_factory(make_freedv700d_official_mode_factory());
    runner.register_mode_factory(make_freedv700e_official_mode_factory());
  }
  runner.register_mode_factory(
      make_700f_candidate_profile_factory("freedv700f_a_balanced"));
  runner.register_mode_factory(
      make_700f_candidate_profile_factory("freedv700f_b_robust"));
  runner.register_mode_factory(
      make_700f_candidate_profile_factory("freedv700f_c_quality"));
}

} // namespace f700f

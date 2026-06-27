#include <f700f/simulation_pipeline.hpp>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <utility>

namespace f700f {
namespace {

constexpr double kPi = 3.14159265358979323846;

void mark_stage(SimulationResult &result, PipelineStage stage, bool ok, std::string message = {}) {
  auto it = std::find_if(result.stage_statuses.begin(), result.stage_statuses.end(),
                         [stage](const StageStatus &status) { return status.stage == stage; });
  if (it == result.stage_statuses.end()) {
    result.stage_statuses.push_back({stage, ok, std::move(message)});
    return;
  }
  it->ok = ok;
  it->message = std::move(message);
}

std::uint64_t fnv1a_append(std::uint64_t hash, const void *data, std::size_t size) {
  const auto *bytes = static_cast<const unsigned char *>(data);
  for (std::size_t i = 0; i < size; ++i) {
    hash ^= bytes[i];
    hash *= 1099511628211ULL;
  }
  return hash;
}

std::uint64_t fnv1a_append_string(std::uint64_t hash, const std::string &value) {
  return fnv1a_append(hash, value.data(), value.size());
}

template <typename T>
std::uint64_t fnv1a_append_value(std::uint64_t hash, const T &value) {
  return fnv1a_append(hash, &value, sizeof(T));
}

std::string hex_digest(std::uint64_t hash) {
  std::ostringstream out;
  out << std::hex << std::setw(16) << std::setfill('0') << hash;
  return out.str();
}

std::string make_digest(const SimulationResult &result) {
  auto hash = 1469598103934665603ULL;
  hash = fnv1a_append_string(hash, result.run_id);
  hash = fnv1a_append_value(hash, result.seed);
  hash = fnv1a_append_string(hash, result.mode_id);
  for (const auto &channel_id : result.channel_ids) {
    hash = fnv1a_append_string(hash, channel_id);
  }
  for (const auto sample : result.decoded_audio.mono) {
    hash = fnv1a_append_value(hash, sample);
  }
  for (const auto &metric : result.metrics) {
    hash = fnv1a_append_string(hash, metric.metric_id);
    hash = fnv1a_append_value(hash, metric.value);
  }
  return hex_digest(hash);
}

bool has_true_parameter(const std::vector<Capability> &parameters, const std::string &key) {
  return std::any_of(parameters.begin(), parameters.end(), [&key](const Capability &parameter) {
    return parameter.key == key && parameter.value == "true";
  });
}

AudioBlock make_generated_tone(const GeneratedToneConfig &config) {
  AudioBlock audio;
  audio.sample_rate_hz = config.sample_rate_hz;
  audio.mono.reserve(config.sample_count);
  if (config.sample_rate_hz == 0) {
    return audio;
  }
  for (std::uint32_t i = 0; i < config.sample_count; ++i) {
    const auto phase = 2.0 * kPi * static_cast<double>(config.frequency_hz) * static_cast<double>(i) /
                       static_cast<double>(config.sample_rate_hz);
    audio.mono.push_back(config.amplitude * static_cast<float>(std::sin(phase)));
  }
  return audio;
}

AudioBlock load_input_audio(const SimulationInputConfig &input, std::string &error) {
  if (std::holds_alternative<GeneratedToneConfig>(input)) {
    return make_generated_tone(std::get<GeneratedToneConfig>(input));
  }
  const auto &file = std::get<AudioFileInputConfig>(input);
  if (file.path.empty()) {
    error = "audio input path is empty";
    return {};
  }
  error = "audio file input is reserved for later audio IO integration";
  return {};
}

bool write_text_file(const std::filesystem::path &path, const std::string &contents, std::string &error) {
  std::error_code ec;
  std::filesystem::create_directories(path.parent_path(), ec);
  if (ec) {
    error = "failed to create output directory: " + ec.message();
    return false;
  }
  std::ofstream out(path);
  if (!out) {
    error = "failed to open output artifact: " + path.string();
    return false;
  }
  out << contents;
  return true;
}

void emit_summary_artifacts(const SimulationConfig &config, SimulationResult &result) {
  if (config.output_directory.empty() || config.run_id.empty()) {
    return;
  }

  std::string error;
  const auto base = std::filesystem::path(config.output_directory);
  const auto json_path = base / (config.run_id + ".json");
  if (write_text_file(json_path, simulation_result_to_json(result), error)) {
    result.artifacts.push_back({"summary-json", json_path.generic_string(), "application/json"});
  } else {
    mark_stage(result, PipelineStage::Metrics, false, error);
    result.ok = false;
    result.error = error;
  }

  const auto csv_path = base / (config.run_id + ".csv");
  if (write_text_file(csv_path, simulation_result_to_csv(result), error)) {
    result.artifacts.push_back({"summary-csv", csv_path.generic_string(), "text/csv"});
  } else {
    mark_stage(result, PipelineStage::Metrics, false, error);
    result.ok = false;
    result.error = error;
  }

  if (config.export_audio) {
    result.artifacts.push_back({"decoded-audio-placeholder", (base / (config.run_id + ".decoded.raw")).generic_string(), "audio/f32le"});
  }
}

class DummyMode final : public IMode {
public:
  explicit DummyMode(ModeId mode_id) {
    descriptor_.mode_id = std::move(mode_id);
    descriptor_.display_name = "ISSUE-0009 dummy mode";
    descriptor_.sample_rate_hz = 8000;
    descriptor_.rf_bandwidth_hz = 1600.0;
    descriptor_.codec_id = "dummy";
    descriptor_.fec_id = "none";
    descriptor_.modem_id = "dummy-iq";
    descriptor_.capabilities.audio_input = true;
    descriptor_.capabilities.audio_output = true;
    descriptor_.capabilities.complex_input = true;
    descriptor_.capabilities.complex_output = true;
    descriptor_.capabilities.variable_frame_size = true;
  }

  const ModeDescriptor &descriptor() const noexcept override { return descriptor_; }

  bool configure(const ModeRuntimeConfig &config) override {
    configured_ = config.sample_rate_hz == descriptor_.sample_rate_hz;
    return configured_;
  }

  EncodeResult encode(const AudioBlock &audio) override {
    EncodeResult result;
    result.status.frame_index = frame_index_++;
    result.symbols.sample_rate_hz = audio.sample_rate_hz;
    result.symbols.start_time_s = audio.start_time_s;
    if (!configured_) {
      result.error = "dummy mode is not configured";
      return result;
    }
    result.ok = true;
    result.status.sync = true;
    result.status.fec_ok = true;
    result.status.confidence = 1.0F;
    result.symbols.iq.reserve(audio.mono.size());
    for (const auto sample : audio.mono) {
      result.symbols.iq.push_back({sample, -sample});
    }
    return result;
  }

  DecodeResult decode(const ComplexBlock &symbols) override {
    DecodeResult result;
    result.status.frame_index = frame_index_++;
    result.audio.sample_rate_hz = descriptor_.sample_rate_hz;
    result.audio.start_time_s = symbols.start_time_s;
    if (!configured_) {
      result.error = "dummy mode is not configured";
      return result;
    }
    result.ok = true;
    result.status.sync = true;
    result.status.fec_ok = true;
    result.status.confidence = 1.0F;
    result.audio.mono.reserve(symbols.iq.size());
    for (const auto &sample : symbols.iq) {
      result.audio.mono.push_back(sample.re);
    }
    return result;
  }

  void reset() noexcept override { frame_index_ = 0; }

private:
  ModeDescriptor descriptor_;
  bool configured_ = false;
  std::uint64_t frame_index_ = 0;
};

class DummyModeFactory final : public IModeFactory {
public:
  explicit DummyModeFactory(ModeId mode_id) : descriptor_(DummyMode(std::move(mode_id)).descriptor()) {}
  const ModeDescriptor &descriptor() const noexcept override { return descriptor_; }
  std::unique_ptr<IMode> create() const override { return std::make_unique<DummyMode>(descriptor_.mode_id); }

private:
  ModeDescriptor descriptor_;
};

class IdentityChannel final : public IChannel {
public:
  std::string channel_id() const override { return "identity"; }

  bool configure(const ChannelConfig &config, Seed seed, std::string &error) override {
    (void)seed;
    if (config.channel_id != channel_id()) {
      error = "identity channel received config for " + config.channel_id;
      return false;
    }
    if (has_true_parameter(config.parameters, "fail")) {
      error = "identity channel configured to fail";
      return false;
    }
    return true;
  }

  ComplexBlock apply(const ComplexBlock &input, std::string &error) override {
    (void)error;
    return input;
  }
};

class IdentityChannelFactory final : public IChannelFactory {
public:
  std::string channel_id() const override { return "identity"; }
  std::unique_ptr<IChannel> create() const override { return std::make_unique<IdentityChannel>(); }
};

class DummyMetric final : public IMetric {
public:
  std::string metric_id() const override { return "dummy.metric"; }

  bool evaluate(const AudioBlock &reference, const AudioBlock &decoded, MetricValue &metric, std::string &error) const override {
    if (reference.mono.size() != decoded.mono.size()) {
      error = "reference and decoded audio lengths differ";
      return false;
    }
    double total_abs_error = 0.0;
    for (std::size_t i = 0; i < reference.mono.size(); ++i) {
      total_abs_error += std::abs(static_cast<double>(reference.mono[i]) - static_cast<double>(decoded.mono[i]));
    }
    metric.metric_id = metric_id();
    metric.value = reference.mono.empty() ? 0.0 : total_abs_error / static_cast<double>(reference.mono.size());
    return true;
  }
};

} // namespace

const StageStatus &SimulationResult::stage_status(PipelineStage stage) const noexcept {
  const auto it = std::find_if(stage_statuses.begin(), stage_statuses.end(),
                               [stage](const StageStatus &status) { return status.stage == stage; });
  if (it != stage_statuses.end()) {
    return *it;
  }
  static const StageStatus missing{PipelineStage::Input, false, "stage was not run"};
  return missing;
}

bool SimulationRunner::register_mode_factory(std::shared_ptr<IModeFactory> factory) {
  return modes_.register_factory(std::move(factory));
}

bool SimulationRunner::register_channel_factory(std::shared_ptr<IChannelFactory> factory) {
  if (!factory || factory->channel_id().empty()) {
    return false;
  }
  return channel_factories_.emplace(factory->channel_id(), std::move(factory)).second;
}

bool SimulationRunner::register_metric(std::shared_ptr<IMetric> metric) {
  if (!metric || metric->metric_id().empty()) {
    return false;
  }
  return metrics_.emplace(metric->metric_id(), std::move(metric)).second;
}

SimulationResult SimulationRunner::run(const SimulationConfig &config) const {
  SimulationResult result;
  result.run_id = config.run_id;
  result.seed = config.seed;
  result.mode_id = config.mode_id;
  for (const auto &channel : config.channel_chain) {
    result.channel_ids.push_back(channel.channel_id);
  }
  result.stage_statuses = {{PipelineStage::Input, false, {}},   {PipelineStage::Encode, false, {}},
                           {PipelineStage::Channel, false, {}}, {PipelineStage::Decode, false, {}},
                           {PipelineStage::Metrics, false, {}}};

  std::string error;
  result.input_audio = load_input_audio(config.input, error);
  if (!error.empty()) {
    mark_stage(result, PipelineStage::Input, false, error);
    result.error = error;
    result.deterministic_digest = make_digest(result);
    return result;
  }
  mark_stage(result, PipelineStage::Input, true);

  auto mode = modes_.create(config.mode_id);
  if (!mode) {
    error = "mode id not registered: " + config.mode_id;
    mark_stage(result, PipelineStage::Encode, false, error);
    result.error = error;
    result.deterministic_digest = make_digest(result);
    return result;
  }
  if (!mode->configure({.sample_rate_hz = result.input_audio.sample_rate_hz,
                        .max_frame_samples = static_cast<std::uint32_t>(result.input_audio.mono.size())})) {
    error = "mode configure failed: " + config.mode_id;
    mark_stage(result, PipelineStage::Encode, false, error);
    result.error = error;
    result.deterministic_digest = make_digest(result);
    return result;
  }
  const auto encoded = mode->encode(result.input_audio);
  if (!encoded.ok) {
    error = encoded.error.empty() ? "mode encode failed" : encoded.error;
    mark_stage(result, PipelineStage::Encode, false, error);
    result.error = error;
    result.deterministic_digest = make_digest(result);
    return result;
  }
  mark_stage(result, PipelineStage::Encode, true);

  auto channel_output = encoded.symbols;
  for (const auto &channel_config : config.channel_chain) {
    const auto factory_it = channel_factories_.find(channel_config.channel_id);
    if (factory_it == channel_factories_.end()) {
      error = "channel id not registered: " + channel_config.channel_id;
      mark_stage(result, PipelineStage::Channel, false, error);
      result.error = error;
      result.deterministic_digest = make_digest(result);
      return result;
    }
    auto channel = factory_it->second->create();
    if (!channel->configure(channel_config, config.seed, error)) {
      mark_stage(result, PipelineStage::Channel, false, error);
      result.error = error;
      result.deterministic_digest = make_digest(result);
      return result;
    }
    channel_output = channel->apply(channel_output, error);
    if (!error.empty()) {
      mark_stage(result, PipelineStage::Channel, false, error);
      result.error = error;
      result.deterministic_digest = make_digest(result);
      return result;
    }
  }
  result.channel_output = channel_output;
  mark_stage(result, PipelineStage::Channel, true);

  const auto decoded = mode->decode(result.channel_output);
  if (!decoded.ok) {
    error = decoded.error.empty() ? "mode decode failed" : decoded.error;
    mark_stage(result, PipelineStage::Decode, false, error);
    result.error = error;
    result.deterministic_digest = make_digest(result);
    return result;
  }
  result.decoded_audio = decoded.audio;
  mark_stage(result, PipelineStage::Decode, true);

  for (const auto &metric_id : config.metric_ids) {
    const auto metric_it = metrics_.find(metric_id);
    if (metric_it == metrics_.end()) {
      error = "metric id not registered: " + metric_id;
      mark_stage(result, PipelineStage::Metrics, false, error);
      result.error = error;
      result.deterministic_digest = make_digest(result);
      return result;
    }
    MetricValue metric;
    if (!metric_it->second->evaluate(result.input_audio, result.decoded_audio, metric, error)) {
      mark_stage(result, PipelineStage::Metrics, false, error);
      result.error = error;
      result.deterministic_digest = make_digest(result);
      return result;
    }
    result.metrics.push_back(metric);
  }
  mark_stage(result, PipelineStage::Metrics, true);
  result.ok = true;
  result.deterministic_digest = make_digest(result);
  emit_summary_artifacts(config, result);
  return result;
}

std::shared_ptr<IModeFactory> make_dummy_mode_factory(ModeId mode_id) {
  return std::make_shared<DummyModeFactory>(std::move(mode_id));
}

std::shared_ptr<IChannelFactory> make_identity_channel_factory() {
  return std::make_shared<IdentityChannelFactory>();
}

std::shared_ptr<IMetric> make_dummy_metric() {
  return std::make_shared<DummyMetric>();
}

std::string pipeline_stage_name(PipelineStage stage) {
  switch (stage) {
  case PipelineStage::Input:
    return "input";
  case PipelineStage::Encode:
    return "encode";
  case PipelineStage::Channel:
    return "channel";
  case PipelineStage::Decode:
    return "decode";
  case PipelineStage::Metrics:
    return "metrics";
  }
  return "unknown";
}

std::string simulation_result_to_json(const SimulationResult &result) {
  std::ostringstream out;
  out << "{\n";
  out << "  \"ok\": " << (result.ok ? "true" : "false") << ",\n";
  out << "  \"run_id\": \"" << result.run_id << "\",\n";
  out << "  \"seed\": " << result.seed << ",\n";
  out << "  \"mode_id\": \"" << result.mode_id << "\",\n";
  out << "  \"deterministic_digest\": \"" << result.deterministic_digest << "\",\n";
  out << "  \"channels\": [";
  for (std::size_t i = 0; i < result.channel_ids.size(); ++i) {
    out << (i == 0 ? "" : ", ") << "\"" << result.channel_ids[i] << "\"";
  }
  out << "],\n";
  out << "  \"metrics\": [";
  for (std::size_t i = 0; i < result.metrics.size(); ++i) {
    out << (i == 0 ? "" : ", ") << "{\"metric_id\": \"" << result.metrics[i].metric_id << "\", \"value\": "
        << result.metrics[i].value << "}";
  }
  out << "],\n";
  out << "  \"stages\": [";
  for (std::size_t i = 0; i < result.stage_statuses.size(); ++i) {
    const auto &stage = result.stage_statuses[i];
    out << (i == 0 ? "" : ", ") << "{\"stage\": \"" << pipeline_stage_name(stage.stage) << "\", \"ok\": "
        << (stage.ok ? "true" : "false") << ", \"message\": \"" << stage.message << "\"}";
  }
  out << "]\n";
  out << "}\n";
  return out.str();
}

std::string simulation_result_to_csv(const SimulationResult &result) {
  std::ostringstream out;
  out << "run_id,seed,mode_id,channels,digest,metric_id,metric_value,ok\n";
  if (result.metrics.empty()) {
    out << result.run_id << ',' << result.seed << ',' << result.mode_id << ',';
    for (std::size_t i = 0; i < result.channel_ids.size(); ++i) {
      out << (i == 0 ? "" : "|") << result.channel_ids[i];
    }
    out << ',' << result.deterministic_digest << ",,," << (result.ok ? "true" : "false") << '\n';
    return out.str();
  }
  for (const auto &metric : result.metrics) {
    out << result.run_id << ',' << result.seed << ',' << result.mode_id << ',';
    for (std::size_t i = 0; i < result.channel_ids.size(); ++i) {
      out << (i == 0 ? "" : "|") << result.channel_ids[i];
    }
    out << ',' << result.deterministic_digest << ',' << metric.metric_id << ',' << metric.value << ','
        << (result.ok ? "true" : "false") << '\n';
  }
  return out.str();
}

} // namespace f700f

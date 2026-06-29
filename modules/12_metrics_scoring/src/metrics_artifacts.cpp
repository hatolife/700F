#include "f700f/metrics_artifacts.hpp"

#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

std::string escape_json_string(const std::string &value) {
  std::string out;
  out.reserve(value.size() + 8);
  for (char c : value) {
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
    }
  }
  return out;
}

std::string unescape_json_string(const std::string &value) {
  std::string out;
  out.reserve(value.size());
  for (std::size_t i = 0; i < value.size(); ++i) {
    if (value[i] != '\\') {
      out.push_back(value[i]);
      continue;
    }
    if (i + 1 >= value.size()) {
      break;
    }
    char next = value[i + 1];
    switch (next) {
    case '"':
      out.push_back('"');
      break;
    case '\\':
      out.push_back('\\');
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
      out.push_back(next);
      break;
    }
    ++i;
  }
  return out;
}

std::string csv_quote(const std::string &value) {
  bool needs_quote = value.find_first_of(",\"\n\r") != std::string::npos;
  if (!needs_quote) {
    return value;
  }
  std::string out = "\"";
  for (char c : value) {
    if (c == '"') {
      out.push_back('"');
    }
    out.push_back(c);
  }
  out.push_back('"');
  return out;
}

std::vector<std::string> csv_split(const std::string &line) {
  std::vector<std::string> fields;
  std::string current;
  bool in_quotes = false;

  for (std::size_t i = 0; i < line.size(); ++i) {
    const char c = line[i];
    if (in_quotes) {
      if (c == '"') {
        if (i + 1 < line.size() && line[i + 1] == '"') {
          current.push_back('"');
          ++i;
          continue;
        }
        in_quotes = false;
        continue;
      }
      current.push_back(c);
      continue;
    }

    if (c == '"') {
      in_quotes = true;
      continue;
    }

    if (c == ',') {
      fields.push_back(current);
      current.clear();
      continue;
    }

    current.push_back(c);
  }

  fields.push_back(current);
  return fields;
}

std::string to_lower(const std::string &value) {
  std::string out = value;
  std::transform(out.begin(), out.end(), out.begin(),
                 [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
  return out;
}

std::vector<std::string> split_warning_list(const std::string &value) {
  if (value.empty()) {
    return {};
  }
  std::vector<std::string> warnings;
  std::string current;
  bool escape = false;
  for (char c : value) {
    if (escape) {
      current.push_back(c);
      escape = false;
      continue;
    }
    if (c == '\\') {
      escape = true;
      continue;
    }
    if (c == ';') {
      warnings.push_back(current);
      current.clear();
      continue;
    }
    current.push_back(c);
  }
  warnings.push_back(current);
  return warnings;
}

std::string join_warning_list(const std::vector<std::string> &warnings) {
  std::string out;
  for (std::size_t i = 0; i < warnings.size(); ++i) {
    if (i > 0) {
      out.push_back(';');
    }
    for (char c : warnings[i]) {
      if (c == ';' || c == '\\') {
        out.push_back('\\');
      }
      out.push_back(c);
    }
  }
  return out;
}

bool starts_with(const std::string_view value, const std::string_view prefix) {
  return value.size() >= prefix.size() &&
         value.compare(0, prefix.size(), prefix) == 0;
}

std::string strip_quotes(std::string value) {
  if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
    value = value.substr(1, value.size() - 2);
  }
  return value;
}

std::uint64_t parse_u64(const std::string &value, std::uint64_t fallback = 0) {
  if (value.empty()) {
    return fallback;
  }
  const auto cleaned = strip_quotes(value);
  try {
    return std::stoull(cleaned);
  } catch (const std::exception &) {
    return fallback;
  }
}

std::uint32_t parse_u32(const std::string &value, std::uint32_t fallback = 0) {
  return static_cast<std::uint32_t>(parse_u64(value, fallback));
}

double parse_double(const std::string &value, double fallback = 0.0) {
  if (value.empty()) {
    return fallback;
  }
  const auto cleaned = strip_quotes(value);
  try {
    return std::stod(cleaned);
  } catch (const std::exception &) {
    return fallback;
  }
}

bool parse_bool(const std::string &value, bool fallback = false) {
  const auto cleaned = to_lower(strip_quotes(value));
  if (cleaned == "true" || cleaned == "1" || cleaned == "yes" ||
      cleaned == "on") {
    return true;
  }
  if (cleaned == "false" || cleaned == "0" || cleaned == "no" ||
      cleaned == "off") {
    return false;
  }
  return fallback;
}

bool is_surrogate_status(const std::string &status) {
  return status == "surrogate";
}

bool is_prototype_status(const std::string &status) {
  return status == "waveform_prototype" || status == "real_modem_prototype";
}

bool is_non_performance_status(const std::string &status) {
  return status == "surrogate" || status == "profile_only" ||
         status == "descriptor_only" || status == "descriptor-only" ||
         status == "toy" || status == "waveform_prototype" ||
         status == "real_modem_prototype" ||
         status == "emulated_surrogate";
}

void apply_surrogate_snapshot_defaults(
    f700f::metrics::ModeDescriptorSnapshot &snapshot) {
  if (snapshot.implementation_classification.empty()) {
    snapshot.implementation_classification =
        snapshot.implementation_status.empty()
            ? (snapshot.official_baseline ? "official" : "unknown")
            : snapshot.implementation_status;
  }
  if (is_non_performance_status(snapshot.implementation_status)) {
    snapshot.downselect_valid = false;
    snapshot.not_downselect_valid = true;
    snapshot.performance_valid = false;
  }
  snapshot.downselect_validity =
      snapshot.downselect_valid ? "valid" : "invalid";
  if (snapshot.performance_valid) {
    snapshot.performance_validity = "valid";
  } else if (snapshot.performance_validity.empty() ||
             snapshot.performance_validity == "valid") {
    snapshot.performance_validity =
        is_prototype_status(snapshot.implementation_status) ? "limited"
                                                           : "invalid";
  }

  if (!is_surrogate_status(snapshot.implementation_status)) {
    if (is_prototype_status(snapshot.implementation_status)) {
      snapshot.prototype = true;
      snapshot.not_final_modem = true;
      if (snapshot.implementation_status == "waveform_prototype") {
        snapshot.waveform_capable = true;
        if (snapshot.modem_family.empty()) {
          snapshot.modem_family = "toy_audio_waveform";
        }
      } else {
        snapshot.waveform_capable = true;
        if (snapshot.modem_family.empty()) {
          snapshot.modem_family = "minimal_qpsk";
        }
      }
      if (snapshot.codec_family.empty()) {
        snapshot.codec_family = "synthetic";
      }
      if (snapshot.fec_family.empty()) {
        snapshot.fec_family = "none";
      }
      if (snapshot.sync_family.empty()) {
        snapshot.sync_family =
            snapshot.implementation_status == "real_modem_prototype" ? "none"
                                                                      : "";
      }
      if (snapshot.prototype_limitations.empty()) {
        snapshot.prototype_limitations =
            snapshot.implementation_status == "real_modem_prototype"
                ? "ISSUE-0042 minimal QPSK-like baseband prototype; synthetic codec; no FEC; no final synchronization; not final modem; not official FreeDV; not valid for real downselect"
                : "toy audio waveform; synthetic codec; no FEC; not final modem; not official FreeDV; not valid for real downselect";
      }
      if (snapshot.prototype_warning.empty()) {
        snapshot.prototype_warning =
            snapshot.implementation_status == "real_modem_prototype"
                ? "REAL MODEM PROTOTYPE WARNING: limited diagnostics only; performance_valid=limited; not real performance; downselect_valid=false"
                : "WAVEFORM PROTOTYPE WARNING: readiness evidence only; not real performance; downselect_valid=false";
      }
    }
    return;
  }

  snapshot.not_real_modem = true;
  if (snapshot.surrogate_model_name.empty()) {
    snapshot.surrogate_model_name = "700f_candidate_minimal_behavior";
  }
  if (snapshot.surrogate_model_version.empty()) {
    snapshot.surrogate_model_version = "ISSUE-0032-v1";
  }
  if (snapshot.surrogate_limitations.empty()) {
    snapshot.surrogate_limitations =
        "synthetic readiness only; not a real modem; BER/FER are not emitted as real values";
  }
}

std::optional<double> parse_optional_double(const std::string &value) {
  const auto cleaned = to_lower(strip_quotes(value));
  if (cleaned.empty() || cleaned == "na" || cleaned == "n/a" ||
      cleaned == "\"n/a\"" || cleaned == "null") {
    return std::nullopt;
  }
  try {
    return std::stod(cleaned);
  } catch (const std::exception &) {
    return std::nullopt;
  }
}

std::string extract_quoted_field(const std::string &json,
                                const std::string &key) {
  const auto pos = json.find("\"" + key + "\"");
  if (pos == std::string::npos) {
    return {};
  }
  auto value_start = json.find(':', pos);
  if (value_start == std::string::npos) {
    return {};
  }
  ++value_start;
  while (value_start < json.size() &&
         std::isspace(static_cast<unsigned char>(json[value_start]))) {
    ++value_start;
  }
  if (value_start >= json.size() || json[value_start] != '"') {
    return {};
  }
  ++value_start;
  std::string out;
  for (std::size_t i = value_start; i < json.size(); ++i) {
    if (json[i] == '\\' && i + 1 < json.size()) {
      out.push_back(json[i]);
      out.push_back(json[i + 1]);
      ++i;
      continue;
    }
    if (json[i] == '"') {
      return unescape_json_string(out);
    }
    out.push_back(json[i]);
  }
  return {};
}

std::string extract_quoted_field_after(const std::string &json,
                                      const std::string &key,
                                      const std::string &after_key) {
  const auto after_pos = json.find("\"" + after_key + "\"");
  if (after_pos == std::string::npos) {
    return extract_quoted_field(json, key);
  }
  return extract_quoted_field(json.substr(after_pos), key);
}

std::string find_object_for_key(const std::string &json, const std::string &key) {
  const auto pos = json.find("\"" + key + "\"");
  if (pos == std::string::npos) {
    return {};
  }
  auto i = json.find(':', pos);
  if (i == std::string::npos) {
    return {};
  }
  ++i;
  while (i < json.size() && std::isspace((unsigned char)json[i])) {
    ++i;
  }
  if (i >= json.size() || json[i] != '{') {
    return {};
  }
  std::size_t end = i + 1;
  int braces = 1;
  for (; end < json.size(); ++end) {
    if (json[end] == '"') {
      ++end;
      while (end < json.size()) {
        if (json[end] == '\\') {
          ++end;
        } else if (json[end] == '"') {
          break;
        }
        ++end;
      }
      continue;
    }
    if (json[end] == '{') {
      ++braces;
    } else if (json[end] == '}') {
      --braces;
      if (braces == 0) {
        return json.substr(i, end - i + 1);
      }
    }
  }
  return {};
}

std::string extract_raw_numeric_or_null(const std::string &json,
                                       const std::string &key) {
  const auto pos = json.find("\"" + key + "\"");
  if (pos == std::string::npos) {
    return {};
  }
  auto i = json.find(':', pos);
  if (i == std::string::npos) {
    return {};
  }
  ++i;
  while (i < json.size() && std::isspace((unsigned char)json[i])) {
    ++i;
  }
  if (i >= json.size()) {
    return {};
  }
  if (starts_with(std::string_view(json).substr(i), "null")) {
    return "null";
  }
  const auto end = json.find_first_of(",}", i);
  if (end == std::string::npos) {
    return {};
  }
  std::string out = json.substr(i, end - i);
  out.erase(std::remove_if(out.begin(), out.end(), [](char c) {
              return c == ' ' || c == '\n' || c == '\t' || c == '\r';
            }),
            out.end());
  return out;
}

std::string extract_raw_array_string(const std::string &json,
                                    const std::string &key) {
  const auto pos = json.find("\"" + key + "\"");
  if (pos == std::string::npos) {
    return {};
  }
  auto i = json.find(':', pos);
  if (i == std::string::npos) {
    return {};
  }
  ++i;
  while (i < json.size() && std::isspace((unsigned char)json[i])) {
    ++i;
  }
  if (i >= json.size() || json[i] != '[') {
    return {};
  }
  auto end = json.find(']', i);
  if (end == std::string::npos) {
    return {};
  }
  return json.substr(i, end - i + 1);
}

std::map<std::string, std::string> parse_string_map(const std::string &json_object) {
  std::map<std::string, std::string> values;
  auto i = json_object.find('{');
  if (i == std::string::npos) {
    return values;
  }
  ++i;
  while (i < json_object.size()) {
    while (i < json_object.size() &&
           std::isspace((unsigned char)json_object[i])) {
      ++i;
    }
    if (i >= json_object.size() || json_object[i] == '}') {
      break;
    }
    if (json_object[i] != '"') {
      ++i;
      continue;
    }
    ++i;
    std::string key;
    for (; i < json_object.size(); ++i) {
      if (json_object[i] == '\\' && i + 1 < json_object.size()) {
        key.push_back(json_object[i + 1]);
        ++i;
        continue;
      }
      if (json_object[i] == '"') {
        break;
      }
      key.push_back(json_object[i]);
    }
    key = unescape_json_string(key);
    ++i;
    i = json_object.find(':', i);
    if (i == std::string::npos) {
      break;
    }
    ++i;
    while (i < json_object.size() &&
           std::isspace((unsigned char)json_object[i])) {
      ++i;
    }

    std::string value;
    if (json_object[i] == '"') {
      ++i;
      for (; i < json_object.size(); ++i) {
        if (json_object[i] == '\\' && i + 1 < json_object.size()) {
          value.push_back(json_object[i + 1]);
          ++i;
          continue;
        }
        if (json_object[i] == '"') {
          break;
        }
        value.push_back(json_object[i]);
      }
      value = unescape_json_string(value);
    } else {
      auto end = json_object.find_first_of(",}", i);
      if (end == std::string::npos) {
        break;
      }
      value = json_object.substr(i, end - i);
      value.erase(std::remove_if(value.begin(), value.end(),
                                 [](char c) {
                                   return c == ' ' || c == '\n' || c == '\t' ||
                                          c == '\r';
                                 }),
                  value.end());
    }
    values.emplace(std::move(key), std::move(value));

    auto next = json_object.find(',', i);
    if (next == std::string::npos) {
      break;
    }
    i = next + 1;
  }
  return values;
}

std::vector<std::string> parse_json_array_strings(const std::string &json_array) {
  std::vector<std::string> values;
  auto i = json_array.find('[');
  if (i == std::string::npos) {
    return values;
  }
  ++i;
  while (i < json_array.size()) {
    while (i < json_array.size() &&
           std::isspace((unsigned char)json_array[i])) {
      ++i;
    }
    if (i >= json_array.size() || json_array[i] == ']') {
      break;
    }
    if (json_array[i] != '"') {
      ++i;
      continue;
    }
    ++i;
    std::string value;
    for (; i < json_array.size(); ++i) {
      if (json_array[i] == '\\' && i + 1 < json_array.size()) {
        value.push_back(json_array[i + 1]);
        ++i;
        continue;
      }
      if (json_array[i] == '"') {
        break;
      }
      value.push_back(json_array[i]);
    }
    values.push_back(unescape_json_string(value));
    ++i;
    auto next = json_array.find(',', i);
    if (next == std::string::npos) {
      break;
    }
    i = next + 1;
  }
  return values;
}

void append_json_key_value(std::string &out, const std::string &key,
                          const std::string &value) {
  out += '"';
  out += key;
  out += "\":\"";
  out += escape_json_string(value);
  out += '"';
}

std::string optional_to_json(const std::optional<double> &value) {
  return value ? std::to_string(*value) : "\"N/A\"";
}

std::string bool_to_json(bool value) { return value ? "true" : "false"; }

std::string to_string_with_precision(double value) {
  std::ostringstream out;
  out << value;
  return out.str();
}

}  // namespace

namespace f700f::metrics {

ModeDescriptorSnapshot make_mode_descriptor_snapshot(
    const f700f::ModeDescriptor &descriptor) {
  ModeDescriptorSnapshot snapshot;
  snapshot.mode_id = descriptor.mode_id;
  snapshot.display_name = descriptor.display_name;
  snapshot.sample_rate_hz = descriptor.sample_rate_hz;
  snapshot.rf_bandwidth_hz = descriptor.rf_bandwidth_hz;
  snapshot.audio_low_hz = descriptor.audio_low_hz;
  snapshot.audio_high_hz = descriptor.audio_high_hz;
  snapshot.nominal_latency_s = descriptor.nominal_latency_s;
  snapshot.frame_duration_s = descriptor.frame_duration_s;
  snapshot.raw_bitrate_bps = descriptor.raw_bitrate_bps;
  snapshot.voice_bitrate_bps = descriptor.voice_bitrate_bps;
  snapshot.codec_id = descriptor.codec_id;
  snapshot.fec_id = descriptor.fec_id;
  snapshot.modem_id = descriptor.modem_id;
  snapshot.audio_bandwidth_hz = descriptor.audio_bandwidth_hz;
  snapshot.official_baseline = descriptor.official_baseline;
  snapshot.emulator = descriptor.emulator;
  snapshot.implementation_status = descriptor.implementation_status;
  apply_surrogate_snapshot_defaults(snapshot);
  snapshot.supports_audio_input = descriptor.capabilities.audio_input;
  snapshot.supports_audio_output = descriptor.capabilities.audio_output;
  snapshot.supports_complex_input = descriptor.capabilities.complex_input;
  snapshot.supports_complex_output = descriptor.capabilities.complex_output;
  snapshot.supports_bit_payload = descriptor.capabilities.bit_payload;
  return snapshot;
}

ResultArtifact make_empty_result_artifact() { return ResultArtifact{}; }

std::vector<std::string> metric_column_names(const ResultArtifact &result) {
  std::vector<std::string> columns;
  for (const auto &[key, _] : result.optional_metrics) {
    columns.emplace_back("opt." + key);
  }
  return columns;
}

std::string json_mode_snapshot(const ModeDescriptorSnapshot &snapshot) {
  std::string out = "{";
  append_json_key_value(out, "mode_id", snapshot.mode_id);
  out.push_back(',');
  append_json_key_value(out, "display_name", snapshot.display_name);
  out.push_back(',');
  out += "\"sample_rate_hz\":" + std::to_string(snapshot.sample_rate_hz);
  out.push_back(',');
  out += "\"rf_bandwidth_hz\":" + to_string_with_precision(snapshot.rf_bandwidth_hz);
  out.push_back(',');
  out += "\"audio_low_hz\":" + to_string_with_precision(snapshot.audio_low_hz);
  out.push_back(',');
  out += "\"audio_high_hz\":" + to_string_with_precision(snapshot.audio_high_hz);
  out.push_back(',');
  out += "\"nominal_latency_s\":" +
         to_string_with_precision(snapshot.nominal_latency_s);
  out.push_back(',');
  out += "\"frame_duration_s\":" +
         to_string_with_precision(snapshot.frame_duration_s);
  out.push_back(',');
  out += "\"raw_bitrate_bps\":" + std::to_string(snapshot.raw_bitrate_bps);
  out.push_back(',');
  out += "\"voice_bitrate_bps\":" + std::to_string(snapshot.voice_bitrate_bps);
  out.push_back(',');
  append_json_key_value(out, "codec_id", snapshot.codec_id);
  out.push_back(',');
  append_json_key_value(out, "fec_id", snapshot.fec_id);
  out.push_back(',');
  append_json_key_value(out, "modem_id", snapshot.modem_id);
  out.push_back(',');
  out += "\"audio_bandwidth_hz\":" +
         to_string_with_precision(snapshot.audio_bandwidth_hz);
  out.push_back(',');
  out += std::string("\"official_baseline\":") +
         bool_to_json(snapshot.official_baseline);
  out.push_back(',');
  out += "\"emulator\":" + bool_to_json(snapshot.emulator);
  out.push_back(',');
  append_json_key_value(out, "implementation_status",
                        snapshot.implementation_status);
  out.push_back(',');
  append_json_key_value(out, "implementation_classification",
                        snapshot.implementation_classification);
  out.push_back(',');
  out += "\"not_real_modem\":" + bool_to_json(snapshot.not_real_modem);
  out.push_back(',');
  out += "\"downselect_valid\":" + bool_to_json(snapshot.downselect_valid);
  out.push_back(',');
  out += "\"not_downselect_valid\":" +
         bool_to_json(snapshot.not_downselect_valid);
  out.push_back(',');
  out += "\"performance_valid\":" + bool_to_json(snapshot.performance_valid);
  out.push_back(',');
  append_json_key_value(out, "performance_validity",
                        snapshot.performance_validity);
  out.push_back(',');
  append_json_key_value(out, "downselect_validity",
                        snapshot.downselect_validity);
  out.push_back(',');
  out += "\"prototype\":" + bool_to_json(snapshot.prototype);
  out.push_back(',');
  out += "\"not_final_modem\":" + bool_to_json(snapshot.not_final_modem);
  out.push_back(',');
  out += "\"waveform_capable\":" + bool_to_json(snapshot.waveform_capable);
  out.push_back(',');
  append_json_key_value(out, "codec_family", snapshot.codec_family);
  out.push_back(',');
  append_json_key_value(out, "fec_family", snapshot.fec_family);
  out.push_back(',');
  append_json_key_value(out, "sync_family", snapshot.sync_family);
  out.push_back(',');
  append_json_key_value(out, "modem_family", snapshot.modem_family);
  out.push_back(',');
  append_json_key_value(out, "prototype_limitations",
                        snapshot.prototype_limitations);
  out.push_back(',');
  append_json_key_value(out, "prototype_warning",
                        snapshot.prototype_warning);
  out.push_back(',');
  append_json_key_value(out, "surrogate_model_name",
                        snapshot.surrogate_model_name);
  out.push_back(',');
  append_json_key_value(out, "surrogate_model_version",
                        snapshot.surrogate_model_version);
  out.push_back(',');
  append_json_key_value(out, "surrogate_limitations",
                        snapshot.surrogate_limitations);
  out.push_back(',');
  out += std::string("\"supports_audio_input\":") + bool_to_json(snapshot.supports_audio_input);
  out.push_back(',');
  out += "\"supports_audio_output\":" + bool_to_json(snapshot.supports_audio_output);
  out.push_back(',');
  out += "\"supports_complex_input\":" + bool_to_json(snapshot.supports_complex_input);
  out.push_back(',');
  out += "\"supports_complex_output\":" + bool_to_json(snapshot.supports_complex_output);
  out.push_back(',');
  out += "\"supports_bit_payload\":" + bool_to_json(snapshot.supports_bit_payload);
  out.push_back('}');
  return out;
}

std::string json_string_map(const std::map<std::string, std::string> &values) {
  std::string out = "{";
  bool first = true;
  for (const auto &[key, value] : values) {
    if (!first) {
      out.push_back(',');
    }
    first = false;
    append_json_key_value(out, key, value);
  }
  out.push_back('}');
  return out;
}

std::string to_json(const ResultArtifact &result) {
  std::string out = "{";
  append_json_key_value(out, "project_version", result.project_version);
  out.push_back(',');
  append_json_key_value(out, "module_version", result.module_version);
  out.push_back(',');
  append_json_key_value(out, "run_id", result.run_id);
  out.push_back(',');
  out += "\"mode_descriptor\":";
  out += json_mode_snapshot(result.mode_descriptor);
  out.push_back(',');
  append_json_key_value(out, "mode_id", result.mode_descriptor.mode_id);
  out.push_back(',');
  append_json_key_value(out, "channel_id", result.channel_id);
  out.push_back(',');
  out += "\"channel_parameters\":";
  out += json_string_map(result.channel_parameters);
  out.push_back(',');
  out += "\"seed\":" + std::to_string(result.seed);
  out.push_back(',');
  out += "\"snr_db\":" + to_string_with_precision(result.snr_db);
  out.push_back(',');
  out += "\"freq_offset_hz\":" + to_string_with_precision(result.freq_offset_hz);
  out.push_back(',');
  out += "\"frame_count\":" + std::to_string(result.frame_count);
  out.push_back(',');
  out += "\"sample_count\":" + std::to_string(result.sample_count);
  out.push_back(',');
  out += "\"ber\":" + optional_to_json(result.ber);
  out.push_back(',');
  out += "\"fer\":" + optional_to_json(result.fer);
  out.push_back(',');
  out += "\"sync_loss_count\":" + std::to_string(result.sync_loss_count);
  out.push_back(',');
  out += "\"dropout_rate\":" + to_string_with_precision(result.dropout_rate);
  out.push_back(',');
  out += "\"latency_estimate_s\":" + to_string_with_precision(result.latency_estimate_s);
  out.push_back(',');
  append_json_key_value(out, "audio_export_path", result.audio_export_path);
  out.push_back(',');
  out += "\"warnings\":";
  out += "[";
  for (std::size_t i = 0; i < result.warnings.size(); ++i) {
    if (i > 0) {
      out.push_back(',');
    }
    std::string item = "\"";
    item += escape_json_string(result.warnings[i]);
    item += "\"";
    out += item;
  }
  out.push_back(']');
  out.push_back(',');
  if (result.skipped_reason.has_value()) {
    append_json_key_value(out, "skipped_reason", *result.skipped_reason);
  } else {
    out += "\"skipped_reason\":null";
  }
  out.push_back(',');
  if (result.error_summary.has_value()) {
    append_json_key_value(out, "error_summary", *result.error_summary);
  } else {
    out += "\"error_summary\":null";
  }
  out.push_back(',');
  out += "\"prototype_symbol_error_rate\":" +
         optional_to_json(result.prototype_symbol_error_rate);
  out.push_back(',');
  append_json_key_value(out, "prototype_frame_status",
                        result.prototype_frame_status);
  out.push_back(',');
  append_json_key_value(out, "prototype_sync_status",
                        result.prototype_sync_status);
  out.push_back(',');
  out += "\"prototype_baseband_sample_count\":" +
         std::to_string(result.prototype_baseband_sample_count);
  out.push_back(',');
  append_json_key_value(out, "prototype_limitations",
                        result.prototype_limitations);
  out.push_back(',');
  out += "\"occupied_bandwidth_estimate_hz\":" +
         optional_to_json(result.occupied_bandwidth_estimate_hz);
  out.push_back(',');
  out += "\"occupied_bandwidth_low_hz\":" +
         optional_to_json(result.occupied_bandwidth_low_hz);
  out.push_back(',');
  out += "\"occupied_bandwidth_high_hz\":" +
         optional_to_json(result.occupied_bandwidth_high_hz);
  out.push_back(',');
  out += "\"occupied_bandwidth_ratio\":" +
         optional_to_json(result.occupied_bandwidth_ratio);
  out.push_back(',');
  append_json_key_value(out, "occupied_bandwidth_status",
                        result.occupied_bandwidth_status);
  out.push_back(',');
  out += "\"optional_metrics\":";
  out += json_string_map(result.optional_metrics);
  out.push_back('}');
  return out;
}

std::vector<std::string> required_csv_columns() {
  return {
      "project_version", "module_version", "run_id", "mode_id",
      "mode_descriptor_snapshot", "channel_id", "channel_parameters", "seed",
      "snr_db", "freq_offset_hz", "frame_count", "sample_count", "ber", "fer",
      "sync_loss_count", "dropout_rate", "latency_estimate_s",
      "audio_export_path", "warnings", "skipped_reason", "error_summary",
      "prototype_symbol_error_rate", "prototype_frame_status",
      "prototype_sync_status", "prototype_baseband_sample_count",
      "prototype_limitations", "occupied_bandwidth_estimate_hz",
      "occupied_bandwidth_low_hz", "occupied_bandwidth_high_hz",
      "occupied_bandwidth_ratio", "occupied_bandwidth_status"};
}

std::string to_csv_header(const std::vector<std::string> &extra_columns) {
  auto columns = required_csv_columns();
  columns.insert(columns.end(), extra_columns.begin(), extra_columns.end());
  std::string out;
  for (std::size_t i = 0; i < columns.size(); ++i) {
    if (i > 0) {
      out.push_back(',');
    }
    out += csv_quote(columns[i]);
  }
  return out;
}

std::string to_csv_row(const ResultArtifact &result,
                       const std::vector<std::string> &extra_columns) {
  auto columns = required_csv_columns();
  columns.insert(columns.end(), extra_columns.begin(), extra_columns.end());
  std::map<std::string, std::string> optional_values;
  for (const std::string &col : columns) {
    (void)col;
  }
  std::vector<std::string> fields;
  fields.reserve(columns.size());
  fields.push_back(csv_quote(result.project_version));
  fields.push_back(csv_quote(result.module_version));
  fields.push_back(csv_quote(result.run_id));
  fields.push_back(csv_quote(result.mode_descriptor.mode_id));
  fields.push_back(csv_quote(json_mode_snapshot(result.mode_descriptor)));
  fields.push_back(csv_quote(result.channel_id));
  fields.push_back(csv_quote(json_string_map(result.channel_parameters)));
  fields.push_back(std::to_string(result.seed));
  fields.push_back(to_string_with_precision(result.snr_db));
  fields.push_back(to_string_with_precision(result.freq_offset_hz));
  fields.push_back(std::to_string(result.frame_count));
  fields.push_back(std::to_string(result.sample_count));
  fields.push_back(result.ber ? to_string_with_precision(*result.ber) : "N/A");
  fields.push_back(result.fer ? to_string_with_precision(*result.fer) : "N/A");
  fields.push_back(std::to_string(result.sync_loss_count));
  fields.push_back(to_string_with_precision(result.dropout_rate));
  fields.push_back(to_string_with_precision(result.latency_estimate_s));
  fields.push_back(csv_quote(result.audio_export_path));
  fields.push_back(csv_quote(join_warning_list(result.warnings)));
  fields.push_back(csv_quote(result.skipped_reason.value_or("")));
  fields.push_back(csv_quote(result.error_summary.value_or("")));
  fields.push_back(csv_quote(result.prototype_symbol_error_rate
                                 ? to_string_with_precision(
                                       *result.prototype_symbol_error_rate)
                                 : "N/A"));
  fields.push_back(csv_quote(result.prototype_frame_status));
  fields.push_back(csv_quote(result.prototype_sync_status));
  fields.push_back(std::to_string(result.prototype_baseband_sample_count));
  fields.push_back(csv_quote(result.prototype_limitations));
  fields.push_back(csv_quote(result.occupied_bandwidth_estimate_hz
                                 ? to_string_with_precision(
                                       *result.occupied_bandwidth_estimate_hz)
                                 : "N/A"));
  fields.push_back(csv_quote(result.occupied_bandwidth_low_hz
                                 ? to_string_with_precision(
                                       *result.occupied_bandwidth_low_hz)
                                 : "N/A"));
  fields.push_back(csv_quote(result.occupied_bandwidth_high_hz
                                 ? to_string_with_precision(
                                       *result.occupied_bandwidth_high_hz)
                                 : "N/A"));
  fields.push_back(csv_quote(result.occupied_bandwidth_ratio
                                 ? to_string_with_precision(
                                       *result.occupied_bandwidth_ratio)
                                 : "N/A"));
  fields.push_back(csv_quote(result.occupied_bandwidth_status));
  for (const auto &col : extra_columns) {
    if (starts_with(col, "opt.")) {
      const auto key = col.substr(4);
      auto it = result.optional_metrics.find(key);
      if (it == result.optional_metrics.end()) {
        fields.push_back("");
      } else {
        fields.push_back(csv_quote(it->second));
      }
    } else {
      fields.push_back("");
    }
  }
  std::string out;
  for (std::size_t i = 0; i < fields.size(); ++i) {
    if (i > 0) {
      out.push_back(',');
    }
    out += fields[i];
  }
  return out;
}

ResultArtifact from_json(const std::string &json_payload) {
  ResultArtifact result;
  result.project_version = extract_quoted_field(json_payload, "project_version");
  result.module_version = extract_quoted_field(json_payload, "module_version");
  result.run_id = extract_quoted_field(json_payload, "run_id");
  result.mode_descriptor.mode_id = extract_quoted_field(json_payload, "mode_id");
  result.channel_id = extract_quoted_field(json_payload, "channel_id");
  result.seed = parse_u64(extract_raw_numeric_or_null(json_payload, "seed"));
  result.snr_db = parse_double(extract_raw_numeric_or_null(json_payload, "snr_db"));
  result.freq_offset_hz =
      parse_double(extract_raw_numeric_or_null(json_payload, "freq_offset_hz"));
  result.frame_count =
      parse_u64(extract_raw_numeric_or_null(json_payload, "frame_count"));
  result.sample_count =
      parse_u64(extract_raw_numeric_or_null(json_payload, "sample_count"));
  result.ber = parse_optional_double(extract_raw_numeric_or_null(json_payload, "ber"));
  result.fer = parse_optional_double(extract_raw_numeric_or_null(json_payload, "fer"));
  result.sync_loss_count =
      parse_u64(extract_raw_numeric_or_null(json_payload, "sync_loss_count"));
  result.dropout_rate =
      parse_double(extract_raw_numeric_or_null(json_payload, "dropout_rate"));
  result.latency_estimate_s =
      parse_double(extract_raw_numeric_or_null(json_payload, "latency_estimate_s"));
  result.audio_export_path =
      extract_quoted_field(json_payload, "audio_export_path");
  const auto warnings_field = extract_raw_array_string(json_payload, "warnings");
  result.warnings = parse_json_array_strings(warnings_field);

  const std::string skipped_raw = extract_quoted_field(json_payload, "skipped_reason");
  if (!skipped_raw.empty()) {
    result.skipped_reason = skipped_raw;
  }
  const std::string error_raw = extract_quoted_field(json_payload, "error_summary");
  if (!error_raw.empty()) {
    result.error_summary = error_raw;
  }
  result.prototype_symbol_error_rate = parse_optional_double(
      extract_raw_numeric_or_null(json_payload, "prototype_symbol_error_rate"));
  result.prototype_frame_status =
      extract_quoted_field(json_payload, "prototype_frame_status");
  result.prototype_sync_status =
      extract_quoted_field(json_payload, "prototype_sync_status");
  result.prototype_baseband_sample_count = parse_u64(
      extract_raw_numeric_or_null(json_payload,
                                  "prototype_baseband_sample_count"));
  result.prototype_limitations =
      extract_quoted_field_after(json_payload, "prototype_limitations",
                                 "prototype_baseband_sample_count");
  result.occupied_bandwidth_estimate_hz = parse_optional_double(
      extract_raw_numeric_or_null(json_payload,
                                  "occupied_bandwidth_estimate_hz"));
  result.occupied_bandwidth_low_hz = parse_optional_double(
      extract_raw_numeric_or_null(json_payload, "occupied_bandwidth_low_hz"));
  result.occupied_bandwidth_high_hz = parse_optional_double(
      extract_raw_numeric_or_null(json_payload, "occupied_bandwidth_high_hz"));
  result.occupied_bandwidth_ratio = parse_optional_double(
      extract_raw_numeric_or_null(json_payload, "occupied_bandwidth_ratio"));
  result.occupied_bandwidth_status =
      extract_quoted_field(json_payload, "occupied_bandwidth_status");

  const auto channel_blob = find_object_for_key(json_payload, "channel_parameters");
  result.channel_parameters = parse_string_map(channel_blob);

  const auto optional_blob = find_object_for_key(json_payload, "optional_metrics");
  result.optional_metrics = parse_string_map(optional_blob);

  const auto descriptor_blob = find_object_for_key(json_payload, "mode_descriptor");
  if (!descriptor_blob.empty()) {
    const auto descriptor_map = parse_string_map(descriptor_blob);
    auto it_mode = descriptor_map.find("mode_id");
    if (it_mode != descriptor_map.end()) {
      result.mode_descriptor.mode_id = it_mode->second;
    }
    auto it_display = descriptor_map.find("display_name");
    if (it_display != descriptor_map.end()) {
      result.mode_descriptor.display_name = it_display->second;
    }
    if (const auto it = descriptor_map.find("sample_rate_hz");
        it != descriptor_map.end()) {
      result.mode_descriptor.sample_rate_hz = parse_u32(it->second);
    }
    if (const auto it = descriptor_map.find("rf_bandwidth_hz");
        it != descriptor_map.end()) {
      result.mode_descriptor.rf_bandwidth_hz = parse_double(it->second);
    }
    if (const auto it = descriptor_map.find("audio_low_hz");
        it != descriptor_map.end()) {
      result.mode_descriptor.audio_low_hz = parse_double(it->second);
    }
    if (const auto it = descriptor_map.find("audio_high_hz");
        it != descriptor_map.end()) {
      result.mode_descriptor.audio_high_hz = parse_double(it->second);
    }
    if (const auto it = descriptor_map.find("nominal_latency_s");
        it != descriptor_map.end()) {
      result.mode_descriptor.nominal_latency_s = parse_double(it->second);
    }
    if (const auto it = descriptor_map.find("frame_duration_s");
        it != descriptor_map.end()) {
      result.mode_descriptor.frame_duration_s = parse_double(it->second);
    }
    if (const auto it = descriptor_map.find("raw_bitrate_bps");
        it != descriptor_map.end()) {
      result.mode_descriptor.raw_bitrate_bps = parse_u32(it->second);
    }
    if (const auto it = descriptor_map.find("voice_bitrate_bps");
        it != descriptor_map.end()) {
      result.mode_descriptor.voice_bitrate_bps = parse_u32(it->second);
    }
    auto it_cap = descriptor_map.find("codec_id");
    if (it_cap != descriptor_map.end()) {
      result.mode_descriptor.codec_id = it_cap->second;
    }
    it_cap = descriptor_map.find("fec_id");
    if (it_cap != descriptor_map.end()) {
      result.mode_descriptor.fec_id = it_cap->second;
    }
    it_cap = descriptor_map.find("modem_id");
    if (it_cap != descriptor_map.end()) {
      result.mode_descriptor.modem_id = it_cap->second;
    }
    if (const auto it = descriptor_map.find("audio_bandwidth_hz");
        it != descriptor_map.end()) {
      result.mode_descriptor.audio_bandwidth_hz = parse_double(it->second);
    }
    if (const auto it = descriptor_map.find("official_baseline");
        it != descriptor_map.end()) {
      result.mode_descriptor.official_baseline = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("emulator");
        it != descriptor_map.end()) {
      result.mode_descriptor.emulator = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("implementation_status");
        it != descriptor_map.end()) {
      result.mode_descriptor.implementation_status = it->second;
    }
    if (const auto it = descriptor_map.find("implementation_classification");
        it != descriptor_map.end()) {
      result.mode_descriptor.implementation_classification = it->second;
    }
    if (const auto it = descriptor_map.find("not_real_modem");
        it != descriptor_map.end()) {
      result.mode_descriptor.not_real_modem = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("downselect_valid");
        it != descriptor_map.end()) {
      result.mode_descriptor.downselect_valid = parse_bool(it->second, true);
    }
    if (const auto it = descriptor_map.find("not_downselect_valid");
        it != descriptor_map.end()) {
      result.mode_descriptor.not_downselect_valid = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("performance_valid");
        it != descriptor_map.end()) {
      result.mode_descriptor.performance_valid = parse_bool(it->second, true);
    }
    if (const auto it = descriptor_map.find("performance_validity");
        it != descriptor_map.end()) {
      result.mode_descriptor.performance_validity = it->second;
    }
    if (const auto it = descriptor_map.find("downselect_validity");
        it != descriptor_map.end()) {
      result.mode_descriptor.downselect_validity = it->second;
    }
    if (const auto it = descriptor_map.find("prototype");
        it != descriptor_map.end()) {
      result.mode_descriptor.prototype = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("not_final_modem");
        it != descriptor_map.end()) {
      result.mode_descriptor.not_final_modem = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("waveform_capable");
        it != descriptor_map.end()) {
      result.mode_descriptor.waveform_capable = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("codec_family");
        it != descriptor_map.end()) {
      result.mode_descriptor.codec_family = it->second;
    }
    if (const auto it = descriptor_map.find("fec_family");
        it != descriptor_map.end()) {
      result.mode_descriptor.fec_family = it->second;
    }
    if (const auto it = descriptor_map.find("sync_family");
        it != descriptor_map.end()) {
      result.mode_descriptor.sync_family = it->second;
    }
    if (const auto it = descriptor_map.find("modem_family");
        it != descriptor_map.end()) {
      result.mode_descriptor.modem_family = it->second;
    }
    if (const auto it = descriptor_map.find("prototype_limitations");
        it != descriptor_map.end()) {
      result.mode_descriptor.prototype_limitations = it->second;
    }
    if (const auto it = descriptor_map.find("prototype_warning");
        it != descriptor_map.end()) {
      result.mode_descriptor.prototype_warning = it->second;
    }
    if (const auto it = descriptor_map.find("surrogate_model_name");
        it != descriptor_map.end()) {
      result.mode_descriptor.surrogate_model_name = it->second;
    }
    if (const auto it = descriptor_map.find("surrogate_model_version");
        it != descriptor_map.end()) {
      result.mode_descriptor.surrogate_model_version = it->second;
    }
    if (const auto it = descriptor_map.find("surrogate_limitations");
        it != descriptor_map.end()) {
      result.mode_descriptor.surrogate_limitations = it->second;
    }
    if (const auto it = descriptor_map.find("supports_audio_input");
        it != descriptor_map.end()) {
      result.mode_descriptor.supports_audio_input = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("supports_audio_output");
        it != descriptor_map.end()) {
      result.mode_descriptor.supports_audio_output = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("supports_complex_input");
        it != descriptor_map.end()) {
      result.mode_descriptor.supports_complex_input = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("supports_complex_output");
        it != descriptor_map.end()) {
      result.mode_descriptor.supports_complex_output = parse_bool(it->second);
    }
    if (const auto it = descriptor_map.find("supports_bit_payload");
        it != descriptor_map.end()) {
      result.mode_descriptor.supports_bit_payload = parse_bool(it->second);
    }
    apply_surrogate_snapshot_defaults(result.mode_descriptor);
  }

  return result;
}

ResultArtifact from_csv_row(const std::string &header_line,
                           const std::string &row_line) {
  const auto headers = csv_split(header_line);
  const auto fields = csv_split(row_line);
  if (headers.size() != fields.size()) {
    throw std::runtime_error("header and row sizes mismatch");
  }

  ResultArtifact result;
  for (std::size_t i = 0; i < headers.size(); ++i) {
    const auto &name = headers[i];
    const auto &value = fields[i];
    if (name == "project_version") {
      result.project_version = value;
    } else if (name == "module_version") {
      result.module_version = value;
    } else if (name == "run_id") {
      result.run_id = value;
    } else if (name == "mode_id") {
      result.mode_descriptor.mode_id = value;
    } else if (name == "channel_id") {
      result.channel_id = value;
    } else if (name == "seed") {
      result.seed = parse_u64(value);
    } else if (name == "snr_db") {
      result.snr_db = parse_double(value);
    } else if (name == "freq_offset_hz") {
      result.freq_offset_hz = parse_double(value);
    } else if (name == "frame_count") {
      result.frame_count = parse_u64(value);
    } else if (name == "sample_count") {
      result.sample_count = parse_u64(value);
    } else if (name == "ber") {
      if (!value.empty() && value != "N/A") {
        result.ber = parse_double(value);
      }
    } else if (name == "fer") {
      if (!value.empty() && value != "N/A") {
        result.fer = parse_double(value);
      }
    } else if (name == "sync_loss_count") {
      result.sync_loss_count = parse_u64(value);
    } else if (name == "dropout_rate") {
      result.dropout_rate = parse_double(value);
    } else if (name == "latency_estimate_s") {
      result.latency_estimate_s = parse_double(value);
    } else if (name == "audio_export_path") {
      result.audio_export_path = value;
    } else if (name == "warnings") {
      result.warnings = split_warning_list(value);
    } else if (name == "skipped_reason") {
      if (!value.empty()) {
        result.skipped_reason = value;
      }
    } else if (name == "error_summary") {
      if (!value.empty()) {
        result.error_summary = value;
      }
    } else if (name == "prototype_symbol_error_rate") {
      result.prototype_symbol_error_rate = parse_optional_double(value);
    } else if (name == "prototype_frame_status") {
      result.prototype_frame_status = value;
    } else if (name == "prototype_sync_status") {
      result.prototype_sync_status = value;
    } else if (name == "prototype_baseband_sample_count") {
      result.prototype_baseband_sample_count = parse_u64(value);
    } else if (name == "prototype_limitations") {
      result.prototype_limitations = value;
    } else if (name == "occupied_bandwidth_estimate_hz") {
      result.occupied_bandwidth_estimate_hz = parse_optional_double(value);
    } else if (name == "occupied_bandwidth_low_hz") {
      result.occupied_bandwidth_low_hz = parse_optional_double(value);
    } else if (name == "occupied_bandwidth_high_hz") {
      result.occupied_bandwidth_high_hz = parse_optional_double(value);
    } else if (name == "occupied_bandwidth_ratio") {
      result.occupied_bandwidth_ratio = parse_optional_double(value);
    } else if (name == "occupied_bandwidth_status") {
      result.occupied_bandwidth_status = value;
    } else if (name == "channel_parameters") {
      result.channel_parameters = parse_string_map(value);
    } else if (name == "mode_descriptor_snapshot") {
      const auto descriptor_map = parse_string_map(value);
      const auto it = descriptor_map.find("mode_id");
      if (it != descriptor_map.end()) {
        result.mode_descriptor.mode_id = it->second;
      }
      const auto it_name = descriptor_map.find("sample_rate_hz");
      if (it_name != descriptor_map.end()) {
        result.mode_descriptor.sample_rate_hz = parse_u32(it_name->second);
      }
      if (const auto it_display = descriptor_map.find("display_name");
          it_display != descriptor_map.end()) {
        result.mode_descriptor.display_name = it_display->second;
      }
      if (const auto it_bw = descriptor_map.find("rf_bandwidth_hz");
          it_bw != descriptor_map.end()) {
        result.mode_descriptor.rf_bandwidth_hz = parse_double(it_bw->second);
      }
      if (const auto it_audio_low = descriptor_map.find("audio_low_hz");
          it_audio_low != descriptor_map.end()) {
        result.mode_descriptor.audio_low_hz = parse_double(it_audio_low->second);
      }
      if (const auto it_audio_high = descriptor_map.find("audio_high_hz");
          it_audio_high != descriptor_map.end()) {
        result.mode_descriptor.audio_high_hz = parse_double(it_audio_high->second);
      }
      if (const auto it_latency = descriptor_map.find("nominal_latency_s");
          it_latency != descriptor_map.end()) {
        result.mode_descriptor.nominal_latency_s = parse_double(it_latency->second);
      }
      if (const auto it_frame = descriptor_map.find("frame_duration_s");
          it_frame != descriptor_map.end()) {
        result.mode_descriptor.frame_duration_s = parse_double(it_frame->second);
      }
      if (const auto it_raw = descriptor_map.find("raw_bitrate_bps");
          it_raw != descriptor_map.end()) {
        result.mode_descriptor.raw_bitrate_bps = parse_u32(it_raw->second);
      }
      if (const auto it_voice = descriptor_map.find("voice_bitrate_bps");
          it_voice != descriptor_map.end()) {
        result.mode_descriptor.voice_bitrate_bps = parse_u32(it_voice->second);
      }
      if (const auto it_codec = descriptor_map.find("codec_id");
          it_codec != descriptor_map.end()) {
        result.mode_descriptor.codec_id = it_codec->second;
      }
      if (const auto it_fec = descriptor_map.find("fec_id");
          it_fec != descriptor_map.end()) {
        result.mode_descriptor.fec_id = it_fec->second;
      }
      if (const auto it_modem = descriptor_map.find("modem_id");
          it_modem != descriptor_map.end()) {
        result.mode_descriptor.modem_id = it_modem->second;
      }
      if (const auto it_audio_bw = descriptor_map.find("audio_bandwidth_hz");
          it_audio_bw != descriptor_map.end()) {
        result.mode_descriptor.audio_bandwidth_hz =
            parse_double(it_audio_bw->second);
      }
      if (const auto it_status = descriptor_map.find("implementation_status");
          it_status != descriptor_map.end()) {
        result.mode_descriptor.implementation_status = it_status->second;
      }
      if (const auto it_class =
              descriptor_map.find("implementation_classification");
          it_class != descriptor_map.end()) {
        result.mode_descriptor.implementation_classification = it_class->second;
      }
      if (const auto it_not_real = descriptor_map.find("not_real_modem");
          it_not_real != descriptor_map.end()) {
        result.mode_descriptor.not_real_modem = parse_bool(it_not_real->second);
      }
      if (const auto it_downselect = descriptor_map.find("downselect_valid");
          it_downselect != descriptor_map.end()) {
        result.mode_descriptor.downselect_valid =
            parse_bool(it_downselect->second, true);
      }
      if (const auto it_not_downselect =
              descriptor_map.find("not_downselect_valid");
          it_not_downselect != descriptor_map.end()) {
        result.mode_descriptor.not_downselect_valid =
            parse_bool(it_not_downselect->second);
      }
      if (const auto it_performance = descriptor_map.find("performance_valid");
          it_performance != descriptor_map.end()) {
        result.mode_descriptor.performance_valid =
            parse_bool(it_performance->second, true);
      }
      if (const auto it_perf_validity =
              descriptor_map.find("performance_validity");
          it_perf_validity != descriptor_map.end()) {
        result.mode_descriptor.performance_validity = it_perf_validity->second;
      }
      if (const auto it_downselect_validity =
              descriptor_map.find("downselect_validity");
          it_downselect_validity != descriptor_map.end()) {
        result.mode_descriptor.downselect_validity =
            it_downselect_validity->second;
      }
      if (const auto it_prototype = descriptor_map.find("prototype");
          it_prototype != descriptor_map.end()) {
        result.mode_descriptor.prototype = parse_bool(it_prototype->second);
      }
      if (const auto it_not_final =
              descriptor_map.find("not_final_modem");
          it_not_final != descriptor_map.end()) {
        result.mode_descriptor.not_final_modem =
            parse_bool(it_not_final->second);
      }
      if (const auto it_waveform = descriptor_map.find("waveform_capable");
          it_waveform != descriptor_map.end()) {
        result.mode_descriptor.waveform_capable =
            parse_bool(it_waveform->second);
      }
      if (const auto it_codec_family = descriptor_map.find("codec_family");
          it_codec_family != descriptor_map.end()) {
        result.mode_descriptor.codec_family = it_codec_family->second;
      }
      if (const auto it_fec_family = descriptor_map.find("fec_family");
          it_fec_family != descriptor_map.end()) {
        result.mode_descriptor.fec_family = it_fec_family->second;
      }
      if (const auto it_sync_family = descriptor_map.find("sync_family");
          it_sync_family != descriptor_map.end()) {
        result.mode_descriptor.sync_family = it_sync_family->second;
      }
      if (const auto it_modem_family = descriptor_map.find("modem_family");
          it_modem_family != descriptor_map.end()) {
        result.mode_descriptor.modem_family = it_modem_family->second;
      }
      if (const auto it_prototype_limitations =
              descriptor_map.find("prototype_limitations");
          it_prototype_limitations != descriptor_map.end()) {
        result.mode_descriptor.prototype_limitations =
            it_prototype_limitations->second;
      }
      if (const auto it_prototype_warning =
              descriptor_map.find("prototype_warning");
          it_prototype_warning != descriptor_map.end()) {
        result.mode_descriptor.prototype_warning =
            it_prototype_warning->second;
      }
      if (const auto it_surrogate_name =
              descriptor_map.find("surrogate_model_name");
          it_surrogate_name != descriptor_map.end()) {
        result.mode_descriptor.surrogate_model_name = it_surrogate_name->second;
      }
      if (const auto it_surrogate_version =
              descriptor_map.find("surrogate_model_version");
          it_surrogate_version != descriptor_map.end()) {
        result.mode_descriptor.surrogate_model_version =
            it_surrogate_version->second;
      }
      if (const auto it_surrogate_limitations =
              descriptor_map.find("surrogate_limitations");
          it_surrogate_limitations != descriptor_map.end()) {
        result.mode_descriptor.surrogate_limitations =
            it_surrogate_limitations->second;
      }
      if (const auto it_official = descriptor_map.find("official_baseline");
          it_official != descriptor_map.end()) {
        result.mode_descriptor.official_baseline = parse_bool(it_official->second);
      }
      if (const auto it_emulator = descriptor_map.find("emulator");
          it_emulator != descriptor_map.end()) {
        result.mode_descriptor.emulator = parse_bool(it_emulator->second);
      }
      if (const auto it_audio_in = descriptor_map.find("supports_audio_input");
          it_audio_in != descriptor_map.end()) {
        result.mode_descriptor.supports_audio_input =
            parse_bool(it_audio_in->second);
      }
      if (const auto it_audio_out = descriptor_map.find("supports_audio_output");
          it_audio_out != descriptor_map.end()) {
        result.mode_descriptor.supports_audio_output =
            parse_bool(it_audio_out->second);
      }
      if (const auto it_complex_in = descriptor_map.find("supports_complex_input");
          it_complex_in != descriptor_map.end()) {
        result.mode_descriptor.supports_complex_input =
            parse_bool(it_complex_in->second);
      }
      if (const auto it_complex_out =
              descriptor_map.find("supports_complex_output");
          it_complex_out != descriptor_map.end()) {
        result.mode_descriptor.supports_complex_output =
            parse_bool(it_complex_out->second);
      }
      if (const auto it_bits = descriptor_map.find("supports_bit_payload");
          it_bits != descriptor_map.end()) {
        result.mode_descriptor.supports_bit_payload = parse_bool(it_bits->second);
      }
      apply_surrogate_snapshot_defaults(result.mode_descriptor);
    } else if (starts_with(name, "opt.")) {
      result.optional_metrics[name.substr(4)] = value;
    }
  }
  return result;
}

}  // namespace f700f::metrics

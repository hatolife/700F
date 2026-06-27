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
      "audio_export_path", "warnings", "skipped_reason", "error_summary"};
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
    } else if (starts_with(name, "opt.")) {
      result.optional_metrics[name.substr(4)] = value;
    }
  }
  return result;
}

}  // namespace f700f::metrics

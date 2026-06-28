#include <f700f/report_artifact_loader.hpp>

#include <f700f/scoring_model.hpp>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace f700f::reporting {
namespace {

struct SweepRow {
  std::string run_id;
  std::string status;
  std::string mode_id;
  std::string condition_id;
  std::uint64_t seed = 0;
  std::string skipped_reason;
  std::string error_summary;
  std::string simulation_digest;
};

constexpr const char *kSurrogateModelName =
    "f700f-minimal-freedv700d700e-surrogate";
constexpr const char *kSurrogateModelVersion = "ISSUE-0033-v1";
constexpr const char *kSurrogateLimitations =
    "deterministic surrogate only; not official FreeDV performance";

bool starts_with(std::string_view value, std::string_view prefix) {
  return value.size() >= prefix.size() && value.substr(0, prefix.size()) == prefix;
}

bool contains(std::string_view value, std::string_view token) {
  return value.find(token) != std::string_view::npos;
}

bool is_emulated_surrogate_row(const SweepRow &row) {
  return contains(row.error_summary, "emulated_surrogate") ||
         contains(row.error_summary, "implementation_status=emulated_surrogate");
}

std::string trim(std::string value) {
  auto is_space = [](unsigned char ch) { return std::isspace(ch) != 0; };
  value.erase(value.begin(), std::find_if(value.begin(), value.end(),
                                          [&](char ch) { return !is_space(ch); }));
  value.erase(std::find_if(value.rbegin(), value.rend(),
                           [&](char ch) { return !is_space(ch); })
                  .base(),
              value.end());
  return value;
}

std::uint64_t parse_u64(const std::string &value,
                        const std::string &field_name) {
  try {
    std::size_t consumed = 0;
    const auto parsed = std::stoull(trim(value), &consumed);
    if (consumed != trim(value).size()) {
      throw std::runtime_error("trailing bytes");
    }
    return parsed;
  } catch (const std::exception &) {
    throw std::runtime_error("Malformed input: expected integer field `" +
                             field_name + "`");
  }
}

std::string unescape_json_string(const std::string &value) {
  std::string out;
  out.reserve(value.size());
  for (std::size_t i = 0; i < value.size(); ++i) {
    if (value[i] != '\\' || i + 1 >= value.size()) {
      out.push_back(value[i]);
      continue;
    }
    const char next = value[++i];
    switch (next) {
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
  }
  return out;
}

std::string extract_quoted_field(const std::string &json,
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
  while (i < json.size() &&
         std::isspace(static_cast<unsigned char>(json[i])) != 0) {
    ++i;
  }
  if (i >= json.size() || json[i] != '"') {
    return {};
  }
  ++i;
  std::string value;
  for (; i < json.size(); ++i) {
    if (json[i] == '\\' && i + 1 < json.size()) {
      value.push_back(json[i]);
      value.push_back(json[++i]);
      continue;
    }
    if (json[i] == '"') {
      return unescape_json_string(value);
    }
    value.push_back(json[i]);
  }
  return {};
}

std::string extract_nullable_string_field(const std::string &json,
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
  while (i < json.size() &&
         std::isspace(static_cast<unsigned char>(json[i])) != 0) {
    ++i;
  }
  if (i >= json.size() || json.compare(i, 4, "null") == 0) {
    return {};
  }
  return extract_quoted_field(json.substr(pos), key);
}

std::string extract_raw_field(const std::string &json, const std::string &key) {
  const auto pos = json.find("\"" + key + "\"");
  if (pos == std::string::npos) {
    return {};
  }
  auto i = json.find(':', pos);
  if (i == std::string::npos) {
    return {};
  }
  ++i;
  while (i < json.size() &&
         std::isspace(static_cast<unsigned char>(json[i])) != 0) {
    ++i;
  }
  const auto end = json.find_first_of(",}", i);
  if (end == std::string::npos) {
    return {};
  }
  return trim(json.substr(i, end - i));
}

std::vector<std::string> split_record_objects(const std::string &json_payload) {
  const auto key_pos = json_payload.find("\"records\"");
  if (key_pos == std::string::npos) {
    throw std::runtime_error("Malformed JSON: expected `records` array");
  }
  auto i = json_payload.find('[', key_pos);
  if (i == std::string::npos) {
    throw std::runtime_error("Malformed JSON: expected `records` array");
  }
  ++i;

  std::vector<std::string> objects;
  while (i < json_payload.size()) {
    while (i < json_payload.size() &&
           std::isspace(static_cast<unsigned char>(json_payload[i])) != 0) {
      ++i;
    }
    if (i < json_payload.size() && json_payload[i] == ']') {
      return objects;
    }
    if (i >= json_payload.size() || json_payload[i] != '{') {
      throw std::runtime_error("Malformed JSON: expected record object");
    }

    const auto object_start = i;
    int depth = 0;
    bool in_string = false;
    for (; i < json_payload.size(); ++i) {
      const char ch = json_payload[i];
      if (in_string) {
        if (ch == '\\') {
          ++i;
        } else if (ch == '"') {
          in_string = false;
        }
        continue;
      }
      if (ch == '"') {
        in_string = true;
      } else if (ch == '{') {
        ++depth;
      } else if (ch == '}') {
        --depth;
        if (depth == 0) {
          objects.push_back(json_payload.substr(object_start, i - object_start + 1));
          ++i;
          break;
        }
      }
    }
    if (depth != 0) {
      throw std::runtime_error("Malformed JSON: unterminated record object");
    }
    while (i < json_payload.size() &&
           std::isspace(static_cast<unsigned char>(json_payload[i])) != 0) {
      ++i;
    }
    if (i < json_payload.size() && json_payload[i] == ',') {
      ++i;
    }
  }
  throw std::runtime_error("Malformed JSON: unterminated `records` array");
}

std::vector<std::string> split_csv_line(const std::string &line) {
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
        } else {
          in_quotes = false;
        }
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
  if (in_quotes) {
    throw std::runtime_error("Malformed CSV: unterminated quoted field");
  }
  fields.push_back(current);
  return fields;
}

std::map<std::string, std::string> csv_row_map(
    const std::vector<std::string> &headers, const std::vector<std::string> &fields,
    std::size_t line_number) {
  if (headers.size() != fields.size()) {
    throw std::runtime_error("Malformed CSV: row " + std::to_string(line_number) +
                             " has " + std::to_string(fields.size()) +
                             " fields; expected " +
                             std::to_string(headers.size()));
  }
  std::map<std::string, std::string> values;
  for (std::size_t i = 0; i < headers.size(); ++i) {
    values.emplace(headers[i], fields[i]);
  }
  return values;
}

std::map<std::string, std::string> parse_semicolon_key_values(
    const std::string &text) {
  std::map<std::string, std::string> values;
  std::size_t begin = 0;
  while (begin < text.size()) {
    const auto end = text.find(';', begin);
    auto part = trim(text.substr(begin, end == std::string::npos
                                           ? std::string::npos
                                           : end - begin));
    const auto colon = part.find(':');
    if (colon != std::string::npos) {
      part = trim(part.substr(colon + 1));
    }
    const auto equals = part.find('=');
    if (equals != std::string::npos) {
      values.emplace(trim(part.substr(0, equals)),
                     trim(part.substr(equals + 1)));
    }
    if (end == std::string::npos) {
      break;
    }
    begin = end + 1;
  }
  return values;
}

f700f::metrics::ModeDescriptorSnapshot descriptor_for_mode(
    const SweepRow &row) {
  f700f::metrics::ModeDescriptorSnapshot descriptor;
  descriptor.mode_id = row.mode_id;
  descriptor.display_name = row.mode_id;
  descriptor.sample_rate_hz = 8000;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3000.0;
  descriptor.nominal_latency_s = 0.16;
  descriptor.frame_duration_s = 0.04;
  descriptor.audio_bandwidth_hz = 2700.0;
  descriptor.supports_audio_input = true;
  descriptor.supports_audio_output = true;

  if (starts_with(row.mode_id, "ssb_")) {
    descriptor.rf_bandwidth_hz =
        contains(row.mode_id, "narrow") ? 1900.0 : 3000.0;
    descriptor.codec_id = "analog-voice";
    descriptor.fec_id = "none";
    descriptor.modem_id = "ssb";
    descriptor.emulator = true;
    descriptor.implementation_status = "emulated";
    descriptor.supports_complex_output = true;
    return descriptor;
  }

  descriptor.rf_bandwidth_hz = 1600.0;
  descriptor.codec_id = "codec2";
  descriptor.fec_id = "unknown";
  descriptor.modem_id = "freedv";
  descriptor.supports_complex_input = true;
  descriptor.supports_complex_output = true;
  descriptor.supports_bit_payload = true;

  if (contains(row.mode_id, "_official")) {
    descriptor.official_baseline = true;
    descriptor.implementation_status = "unavailable";
  } else if (is_emulated_surrogate_row(row)) {
    descriptor.emulator = true;
    descriptor.implementation_status = "emulated_surrogate";
  } else if (starts_with(row.mode_id, "freedv700f_") ||
             contains(row.error_summary, "profile_only")) {
    descriptor.implementation_status = "profile_only";
  } else if (contains(row.mode_id, "_emulated") ||
             contains(row.error_summary, "descriptor_only")) {
    descriptor.emulator = true;
    descriptor.implementation_status = "descriptor_only";
  } else {
    descriptor.implementation_status = "unknown";
  }
  return descriptor;
}

f700f::metrics::ResultArtifact result_from_sweep_row(const SweepRow &row) {
  if (row.run_id.empty() || row.status.empty() || row.mode_id.empty() ||
      row.condition_id.empty()) {
    throw std::runtime_error("Malformed input: sweep row is missing required fields");
  }

  f700f::metrics::ResultArtifact result;
  result.project_version = "v0.2.0";
  result.module_version = "v0.2.0";
  result.run_id = row.run_id;
  result.mode_descriptor = descriptor_for_mode(row);
  result.channel_id = row.condition_id;
  result.seed = row.seed;
  result.frame_count = row.status == "completed" ? 1 : 0;
  result.sample_count = row.status == "completed" ? 1 : 0;
  result.optional_metrics["sweep_status"] = row.status;
  if (!row.simulation_digest.empty()) {
    result.optional_metrics["simulation_digest"] = row.simulation_digest;
  }
  if (!row.error_summary.empty()) {
    result.optional_metrics["sweep_note"] = row.error_summary;
  }
  if (is_emulated_surrogate_row(row)) {
    const auto values = parse_semicolon_key_values(row.error_summary);
    const auto lookup = [&](const std::string &key, const std::string &fallback) {
      const auto it = values.find(key);
      return it == values.end() || it->second.empty() ? fallback : it->second;
    };
    result.optional_metrics["official"] = lookup("official", "false");
    result.optional_metrics["not_official_freedv"] =
        lookup("not_official_freedv", "true");
    result.optional_metrics["downselect_valid"] =
        lookup("downselect_valid", "false");
    result.optional_metrics["performance_valid"] =
        lookup("performance_valid", "false");
    result.optional_metrics["emulator_model_name"] =
        lookup("emulator_model_name", kSurrogateModelName);
    result.optional_metrics["emulator_model_version"] =
        lookup("emulator_model_version", kSurrogateModelVersion);
    result.optional_metrics["emulator_limitations"] =
        lookup("emulator_limitations", kSurrogateLimitations);
    result.warnings.push_back(
        "not_official_freedv=true; performance_valid=false; "
        "downselect_valid=false; emulator_limitations=" +
        result.optional_metrics["emulator_limitations"]);
  }

  if (row.status == "skipped") {
    result.skipped_reason =
        row.skipped_reason.empty() ? "sweep row skipped" : row.skipped_reason;
  } else if (row.status == "failed") {
    result.error_summary =
        row.error_summary.empty() ? "sweep row failed" : row.error_summary;
  } else if (row.status != "completed") {
    throw std::runtime_error("Malformed input: unknown sweep status `" +
                             row.status + "`");
  }
  return result;
}

void finalize_context(LoadedReportInput &loaded,
                      const std::vector<SweepRow> &rows) {
  std::set<std::string> modes;
  std::set<std::string> channels;
  std::set<std::uint64_t> seeds;
  bool saw_profile_only = false;
  bool saw_descriptor_only = false;
  bool saw_emulated_surrogate = false;
  bool saw_skipped = false;
  bool saw_failed = false;

  for (const auto &row : rows) {
    modes.insert(row.mode_id);
    channels.insert(row.condition_id);
    seeds.insert(row.seed);
    saw_profile_only = saw_profile_only || contains(row.error_summary, "profile_only") ||
                       starts_with(row.mode_id, "freedv700f_");
    saw_descriptor_only = saw_descriptor_only ||
                          contains(row.error_summary, "descriptor_only");
    saw_emulated_surrogate =
        saw_emulated_surrogate || is_emulated_surrogate_row(row);
    saw_skipped = saw_skipped || row.status == "skipped";
    saw_failed = saw_failed || row.status == "failed";
  }

  loaded.context.mode_ids.assign(modes.begin(), modes.end());
  loaded.context.channel_ids.assign(channels.begin(), channels.end());
  loaded.context.seeds.assign(seeds.begin(), seeds.end());
  loaded.context.sweep_status =
      saw_failed ? "loaded with failed rows" : "loaded from artifact";
  loaded.real_downselect_possible =
      !saw_profile_only && !saw_descriptor_only && !saw_emulated_surrogate &&
      !saw_skipped && !saw_failed &&
      !rows.empty();
  loaded.context.real_downselect_possible = loaded.real_downselect_possible;
  loaded.context.downselect_feasibility_summary =
      loaded.real_downselect_possible
          ? "Real downselect possible: yes; all loaded rows contain completed "
            "performance evidence."
          : "Real downselect possible: no; skipped, profile-only, "
            "descriptor-only, emulated-surrogate, or failed rows prevent real "
            "downselect.";
  loaded.context.known_limitations.push_back(
      loaded.context.downselect_feasibility_summary);
  if (saw_profile_only) {
    loaded.context.known_limitations.push_back(
        "Profile-only rows are non-performance evidence.");
  }
  if (saw_descriptor_only) {
    loaded.context.known_limitations.push_back(
        "Descriptor-only rows are non-performance evidence.");
  }
  if (saw_emulated_surrogate) {
    loaded.context.known_limitations.push_back(
        "Emulated surrogate rows are non-official and non-performance "
        "evidence: implementation_status=emulated_surrogate; "
        "not_official_freedv=true; downselect_valid=false; "
        "performance_valid=false; emulator_limitations=" +
        std::string(kSurrogateLimitations));
  }
  for (const auto &row : rows) {
    if (!row.skipped_reason.empty()) {
      loaded.context.known_limitations.push_back("Skipped row `" + row.mode_id +
                                                 "`: " + row.skipped_reason);
    }
  }
}

LoadedReportInput loaded_from_rows(std::vector<SweepRow> rows,
                                   std::string run_id) {
  if (rows.empty()) {
    throw std::runtime_error("Malformed input: no sweep records found");
  }
  LoadedReportInput loaded;
  loaded.context.run_id = run_id.empty() ? rows.front().run_id : std::move(run_id);
  loaded.context.commit_hash = "unknown";
  loaded.context.config_path = "unknown";
  loaded.context.generated_at_utc = "unknown";
  loaded.results.reserve(rows.size());
  for (const auto &row : rows) {
    loaded.results.push_back(result_from_sweep_row(row));
  }
  finalize_context(loaded, rows);
  return loaded;
}

std::string read_file(const std::string &path) {
  std::ifstream in(path);
  if (!in) {
    throw std::runtime_error("Unable to read input file: " + path);
  }
  return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

} // namespace

LoadedReportInput load_report_input_json(const std::string &json_payload) {
  const auto text = trim(json_payload);
  if (text.empty() || text.front() != '{' || text.back() != '}') {
    throw std::runtime_error("Malformed JSON: expected top-level object");
  }

  const auto objects = split_record_objects(text);
  std::vector<SweepRow> rows;
  rows.reserve(objects.size());
  for (const auto &object : objects) {
    SweepRow row;
    row.run_id = extract_quoted_field(object, "run_id");
    row.status = extract_quoted_field(object, "status");
    row.mode_id = extract_quoted_field(object, "mode_id");
    row.condition_id = extract_quoted_field(object, "condition_id");
    row.seed = parse_u64(extract_raw_field(object, "seed"), "seed");
    row.skipped_reason = extract_nullable_string_field(object, "skipped_reason");
    row.error_summary = extract_nullable_string_field(object, "error_summary");
    row.simulation_digest = extract_quoted_field(object, "simulation_digest");
    rows.push_back(std::move(row));
  }
  return loaded_from_rows(std::move(rows),
                          extract_quoted_field(text, "run_id_prefix"));
}

LoadedReportInput load_report_input_csv(const std::string &csv_payload) {
  std::istringstream in(csv_payload);
  std::string line;
  if (!std::getline(in, line)) {
    throw std::runtime_error("Malformed CSV: missing header");
  }
  auto headers = split_csv_line(line);
  std::vector<SweepRow> rows;
  std::size_t line_number = 1;
  while (std::getline(in, line)) {
    ++line_number;
    if (trim(line).empty()) {
      continue;
    }
    const auto values = csv_row_map(headers, split_csv_line(line), line_number);
    SweepRow row;
    row.run_id = values.contains("run_id") ? values.at("run_id") : "";
    row.status = values.contains("status") ? values.at("status") : "";
    row.mode_id = values.contains("mode_id") ? values.at("mode_id") : "";
    row.condition_id =
        values.contains("condition_id") ? values.at("condition_id") : "";
    row.seed = parse_u64(values.contains("seed") ? values.at("seed") : "", "seed");
    row.skipped_reason =
        values.contains("skipped_reason") ? values.at("skipped_reason") : "";
    row.error_summary =
        values.contains("error_summary") ? values.at("error_summary") : "";
    row.simulation_digest = values.contains("digest") ? values.at("digest") : "";
    rows.push_back(std::move(row));
  }
  return loaded_from_rows(std::move(rows), {});
}

LoadedReportInput load_report_input_file(const std::string &path) {
  const auto payload = read_file(path);
  const auto extension = std::filesystem::path(path).extension().string();
  if (extension == ".json") {
    return load_report_input_json(payload);
  }
  if (extension == ".csv") {
    return load_report_input_csv(payload);
  }
  throw std::runtime_error("Unsupported input extension: " + extension);
}

std::string render_report_from_loaded_input(const LoadedReportInput &loaded) {
  auto context = loaded.context;
  context.real_downselect_possible = loaded.real_downselect_possible;
  const auto score_report =
      f700f::metrics::score_m2_results(loaded.results, loaded.profile_snapshots);
  return render_m2_baseline_comparison_report(context, score_report);
}

} // namespace f700f::reporting

#include <f700f/baseline_comparison_report.hpp>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>

namespace f700f::reporting {
namespace {

std::string value_or_unknown(const std::string &value) {
  return value.empty() ? "unknown" : value;
}

std::string markdown_escape_cell(const std::string &value) {
  std::string out;
  out.reserve(value.size());
  for (const char c : value) {
    if (c == '|') {
      out += "\\|";
    } else if (c == '\n' || c == '\r') {
      out.push_back(' ');
    } else {
      out.push_back(c);
    }
  }
  return out;
}

std::string code_list(const std::vector<std::string> &values) {
  if (values.empty()) {
    return "none recorded";
  }
  std::ostringstream out;
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i > 0) {
      out << ", ";
    }
    out << "`" << values[i] << "`";
  }
  return out.str();
}

std::string seed_list(const std::vector<std::uint64_t> &values) {
  if (values.empty()) {
    return "none recorded";
  }
  std::ostringstream out;
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i > 0) {
      out << ", ";
    }
    out << "`" << values[i] << "`";
  }
  return out.str();
}

std::string format_double(double value) {
  std::ostringstream out;
  out << std::fixed << std::setprecision(3) << value;
  auto text = out.str();
  while (text.size() > 1 && text.back() == '0') {
    text.pop_back();
  }
  if (!text.empty() && text.back() == '.') {
    text.pop_back();
  }
  return text;
}

std::string descriptor_status(const f700f::metrics::M2ModeScore &score) {
  if (!score.profile_snapshot.has_value()) {
    return "unknown";
  }
  const auto &snapshot = *score.profile_snapshot;
  if (snapshot.official_baseline) {
    return "official";
  }
  if (snapshot.implementation_status == "profile_only") {
    return "profile_only";
  }
  if (snapshot.implementation_status == "surrogate") {
    return "surrogate";
  }
  if (snapshot.implementation_status == "emulated_surrogate") {
    return "emulated_surrogate";
  }
  if (snapshot.implementation_status == "waveform_prototype") {
    return "waveform_prototype";
  }
  if (!snapshot.implementation_status.empty()) {
    return snapshot.implementation_status;
  }
  if (snapshot.emulator) {
    return "emulator";
  }
  return "experimental";
}

bool starts_with(const std::string &value, const std::string &prefix) {
  return value.size() >= prefix.size() &&
         value.compare(0, prefix.size(), prefix) == 0;
}

bool contains_token(const std::string &value, const std::string &token) {
  return value.find(token) != std::string::npos;
}

std::string reason_for_mode(const f700f::metrics::M2ModeScore &score) {
  if (score.official_unavailable_count > 0) {
    return "official adapter unavailable";
  }
  if (score.surrogate) {
    return "SURROGATE WARNING: not_real_modem=true downselect_valid=false "
           "performance_valid=false";
  }
  if (score.profile_only) {
    return "profile_only";
  }
  if (score.profile_snapshot.has_value() &&
      score.profile_snapshot->implementation_status == "waveform_prototype") {
    return "WAVEFORM PROTOTYPE WARNING: prototype=true not_final_modem=true "
           "downselect_valid=false performance_valid=false";
  }
  if (score.performance_invalid_count > 0) {
    return "performance invalid; not_official_freedv=true";
  }
  if (score.skipped_count > 0) {
    return "skipped";
  }
  if (score.failed_count > 0) {
    return "failed";
  }
  return "";
}

void append_mode_rows(std::ostringstream &out,
                      const f700f::metrics::M2ScoreReport &score_report) {
  out << "| Mode | Score | Completed | Failed | Skipped | Official unavailable | "
         "Profile-only | 700F surrogate | Emulated surrogate | "
         "Performance valid | Performance invalid | Real score | "
         "Surrogate readiness | Status | Notes |\n";
  out << "|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---|\n";
  for (const auto &score : score_report.mode_scores) {
    out << "| `" << markdown_escape_cell(score.mode_id) << "` | "
        << format_double(score.score) << " | " << score.completed_count << " | "
        << score.failed_count << " | " << score.skipped_count << " | "
        << score.official_unavailable_count << " | " << score.profile_only_count
        << " | " << score.surrogate_count << " | "
        << score.emulated_surrogate_count << " | "
        << score.performance_valid_count << " | "
        << score.performance_invalid_count << " | "
        << format_double(score.real_performance_score) << " | "
        << format_double(score.surrogate_readiness_score) << " | "
        << descriptor_status(score) << " | "
        << markdown_escape_cell(reason_for_mode(score)) << " |\n";
  }
}

void append_raw_metric_rows(std::ostringstream &out,
                            const f700f::metrics::M2ScoreReport &score_report) {
  out << "| Mode | Records | mean dropout | mean latency s | max RF bandwidth Hz | "
         "BER available | FER available | BER unavailable | FER unavailable | "
         "ASR WER slots | STOI slots | ESTOI slots | Subjective note slots |\n";
  out << "|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|\n";
  for (const auto &score : score_report.mode_scores) {
    out << "| `" << markdown_escape_cell(score.mode_id) << "` | "
        << score.record_count << " | " << format_double(score.mean_dropout_rate)
        << " | " << format_double(score.mean_latency_s) << " | "
        << format_double(score.max_rf_bandwidth_hz) << " | "
        << score.ber_available_count << " | " << score.fer_available_count
        << " | " << score.ber_unavailable_count << " | "
        << score.fer_unavailable_count << " | " << score.asr_wer_slot_count
        << " | " << score.stoi_slot_count << " | " << score.estoi_slot_count
        << " | " << score.subjective_note_slot_count << " |\n";
  }
}

std::uint64_t official_unavailable_total(
    const f700f::metrics::M2ScoreReport &score_report) {
  std::uint64_t total = 0;
  for (const auto &score : score_report.mode_scores) {
    total += score.official_unavailable_count;
  }
  return total;
}

std::uint64_t ssb_completed_total(
    const f700f::metrics::M2ScoreReport &score_report) {
  std::uint64_t total = 0;
  for (const auto &score : score_report.mode_scores) {
    if (starts_with(score.mode_id, "ssb_")) {
      total += score.completed_count;
    }
  }
  return total;
}

std::vector<std::string> candidate_summary_lines(
    const f700f::metrics::M2ScoreReport &score_report) {
  std::vector<std::string> lines;
  for (const auto &score : score_report.mode_scores) {
    if (contains_token(score.mode_id, "700f")) {
      std::string line = "`" + score.mode_id + "` " + descriptor_status(score) +
                         " real score " +
                         format_double(score.real_performance_score) +
                         " surrogate readiness " +
                         format_double(score.surrogate_readiness_score);
      if (score.surrogate && score.profile_snapshot.has_value()) {
        const auto &snapshot = *score.profile_snapshot;
        line += " not_real_modem=";
        line += snapshot.not_real_modem ? "true" : "false";
        line += " downselect_valid=";
        line += snapshot.downselect_valid ? "true" : "false";
        line += " performance_valid=";
        line += snapshot.performance_valid ? "true" : "false";
        line += " synthetic_surrogate_readiness_only";
        if (!snapshot.surrogate_model_name.empty()) {
          line += " model=" + snapshot.surrogate_model_name;
        }
      } else if (score.profile_snapshot.has_value() &&
                 score.profile_snapshot->implementation_status ==
                     "waveform_prototype") {
        const auto &snapshot = *score.profile_snapshot;
        line += " prototype=";
        line += snapshot.prototype ? "true" : "false";
        line += " not_final_modem=";
        line += snapshot.not_final_modem ? "true" : "false";
        line += " waveform_capable=";
        line += snapshot.waveform_capable ? "true" : "false";
        line += " downselect_valid=";
        line += snapshot.downselect_valid ? "true" : "false";
        line += " performance_valid=";
        line += snapshot.performance_valid ? "true" : "false";
        if (!snapshot.modem_family.empty()) {
          line += " modem_family=" + snapshot.modem_family;
        }
      }
      lines.push_back(line);
    }
  }
  return lines;
}

void append_lines_or_placeholder(std::ostringstream &out,
                                 const std::vector<std::string> &lines,
                                 const std::string &placeholder) {
  if (lines.empty()) {
    out << "- " << placeholder << "\n";
    return;
  }
  for (const auto &line : lines) {
    out << "- " << line << "\n";
  }
}

std::string sanitize_run_id(const std::string &run_id) {
  std::string out;
  out.reserve(run_id.size());
  for (const unsigned char ch : run_id) {
    if (std::isalnum(ch) || ch == '-' || ch == '_') {
      out.push_back(static_cast<char>(ch));
    } else {
      out.push_back('_');
    }
  }
  if (out.empty()) {
    return "run";
  }
  return out;
}

std::string compact_timestamp(const std::string &timestamp_utc) {
  std::string out;
  out.reserve(timestamp_utc.size());
  for (const char c : timestamp_utc) {
    if (std::isalnum(static_cast<unsigned char>(c))) {
      out.push_back(c);
    }
  }
  if (out.empty()) {
    return "untimestamped";
  }
  return out;
}

} // namespace

std::string render_m2_baseline_comparison_report(
    const M2BaselineComparisonReportContext &context,
    const f700f::metrics::M2ScoreReport &score_report) {
  std::ostringstream out;
  out << "# M2 Baseline Comparison Report\n\n";
  out << "- Run id: `" << value_or_unknown(context.run_id) << "`\n";
  out << "- Commit: `" << value_or_unknown(context.commit_hash) << "`\n";
  out << "- Config: `" << value_or_unknown(context.config_path) << "`\n";
  out << "- Generated at UTC: `" << value_or_unknown(context.generated_at_utc)
      << "`\n";
  out << "- Score policy: `" << value_or_unknown(score_report.policy_id) << "`\n";
  out << "- Total result records: " << score_report.total_records << "\n";
  out << "- Sweep status: " << value_or_unknown(context.sweep_status) << "\n\n";
  out << "- Downselect feasibility: "
      << value_or_unknown(context.downselect_feasibility_summary) << "\n\n";

  out << "## Mode List\n\n";
  out << code_list(context.mode_ids) << "\n\n";

  out << "## Channel List\n\n";
  out << code_list(context.channel_ids) << "\n\n";

  out << "## Seeds\n\n";
  out << seed_list(context.seeds) << "\n\n";

  out << "## Score Table\n\n";
  if (score_report.mode_scores.empty()) {
    out << "No scored mode entries were available.\n\n";
  } else {
    append_mode_rows(out, score_report);
    out << "\n";
  }

  out << "## Skipped And Unavailable Modes\n\n";
  std::vector<std::string> skipped_lines;
  for (const auto &score : score_report.mode_scores) {
    if (score.skipped_count > 0 || score.official_unavailable_count > 0) {
      skipped_lines.push_back("`" + score.mode_id + "` skipped " +
                              std::to_string(score.skipped_count) +
                              ", official unavailable " +
                              std::to_string(score.official_unavailable_count));
    }
  }
  append_lines_or_placeholder(out, skipped_lines,
                              "No skipped or unavailable modes were scored.");
  out << "\n";

  out << "## Raw Metrics Summary\n\n";
  if (score_report.mode_scores.empty()) {
    out << "No raw metrics were available from scoring input.\n\n";
  } else {
    append_raw_metric_rows(out, score_report);
    out << "\n";
  }

  out << "## Baseline And Candidate Status\n\n";
  out << "- Official FreeDV status: unavailable/skipped: "
      << official_unavailable_total(score_report) << "\n";
  out << "- SSB reference status: completed records: "
      << ssb_completed_total(score_report) << "\n";
  out << "- 700F candidate summary:\n";
  append_lines_or_placeholder(out, candidate_summary_lines(score_report),
                              "No 700F candidate profiles were present.");
  out << "\n";

  out << "## Known Limitations\n\n";
  if (context.known_limitations.empty()) {
    out << "- No additional report limitations were recorded.\n";
  } else {
    for (const auto &limitation : context.known_limitations) {
      out << "- " << limitation << "\n";
    }
  }
  out << "\n";

  out << "## Next recommendation placeholder\n\n";
  out << context.next_recommendation_placeholder << "\n";
  return out.str();
}

std::string make_m2_baseline_report_filename(const std::string &run_id,
                                             const std::string &timestamp_utc) {
  return "m2-baseline-comparison-" + sanitize_run_id(run_id) + "-" +
         compact_timestamp(timestamp_utc) + ".md";
}

} // namespace f700f::reporting

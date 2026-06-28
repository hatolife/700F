#include <f700f/scoring_model.hpp>

#include <algorithm>
#include <cmath>
#include <map>
#include <optional>
#include <sstream>

namespace f700f::metrics {
namespace {

bool is_completed(const ResultArtifact &result) {
  return !result.skipped_reason.has_value() && !result.error_summary.has_value();
}

bool is_skipped(const ResultArtifact &result) {
  return result.skipped_reason.has_value();
}

bool is_failed(const ResultArtifact &result) {
  return !result.skipped_reason.has_value() && result.error_summary.has_value();
}

bool is_profile_only(const ModeDescriptorSnapshot &snapshot) {
  return snapshot.implementation_status == "profile_only";
}

bool is_surrogate(const ModeDescriptorSnapshot &snapshot) {
  return snapshot.implementation_status == "surrogate" ||
         !snapshot.surrogate_model_name.empty();
}

bool is_emulated_surrogate(const ModeDescriptorSnapshot &snapshot) {
  return snapshot.implementation_status == "emulated_surrogate";
}

bool is_descriptor_only(const ModeDescriptorSnapshot &snapshot) {
  return snapshot.implementation_status == "descriptor_only" ||
         snapshot.implementation_status == "descriptor-only";
}

bool is_performance_valid(const ModeDescriptorSnapshot &snapshot) {
  return snapshot.performance_valid && !is_profile_only(snapshot) &&
         !is_surrogate(snapshot) && !is_emulated_surrogate(snapshot) &&
         !is_descriptor_only(snapshot);
}

bool has_optional_slot(const ResultArtifact &result, const std::string &key) {
  return result.optional_metrics.find(key) != result.optional_metrics.end();
}

std::optional<double> optional_metric_as_double(const ResultArtifact &result,
                                                const std::string &key) {
  const auto it = result.optional_metrics.find(key);
  if (it == result.optional_metrics.end()) {
    return std::nullopt;
  }
  try {
    std::size_t consumed = 0;
    const auto value = std::stod(it->second, &consumed);
    if (consumed != it->second.size()) {
      return std::nullopt;
    }
    return value;
  } catch (const std::exception &) {
    return std::nullopt;
  }
}

bool optional_metric_is_false(const ResultArtifact &result, const std::string &key) {
  const auto it = result.optional_metrics.find(key);
  return it != result.optional_metrics.end() && it->second == "false";
}

bool result_performance_valid(const ResultArtifact &result) {
  return is_performance_valid(result.mode_descriptor) &&
         !optional_metric_is_false(result, "performance_valid");
}

double clamp_score(double score) {
  return std::clamp(score, 0.0, 100.0);
}

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

void ensure_profile_snapshot(M2ModeScore &score,
                             const ModeDescriptorSnapshot &snapshot) {
  if (!score.profile_snapshot.has_value()) {
    score.profile_snapshot = snapshot;
  }
  score.profile_only = score.profile_only || is_profile_only(snapshot);
  score.surrogate = score.surrogate || is_surrogate(snapshot);
}

void add_result_to_score(M2ModeScore &score, const ResultArtifact &result) {
  ensure_profile_snapshot(score, result.mode_descriptor);
  const bool performance_valid = result_performance_valid(result);
  ++score.record_count;
  score.mean_dropout_rate += result.dropout_rate;
  score.mean_latency_s += result.latency_estimate_s;
  score.max_rf_bandwidth_hz =
      std::max(score.max_rf_bandwidth_hz, result.mode_descriptor.rf_bandwidth_hz);

  if (is_completed(result)) {
    ++score.completed_count;
    if (performance_valid) {
      ++score.performance_valid_count;
    } else {
      ++score.performance_invalid_count;
    }
  } else if (is_skipped(result)) {
    ++score.skipped_count;
    if (result.mode_descriptor.official_baseline) {
      ++score.official_unavailable_count;
    }
  } else if (is_failed(result)) {
    ++score.failed_count;
  }

  if (is_profile_only(result.mode_descriptor)) {
    ++score.profile_only_count;
  }
  if (is_surrogate(result.mode_descriptor)) {
    ++score.surrogate_count;
    if (const auto readiness =
            optional_metric_as_double(result,
                                      "surrogate_readiness_score_synthetic");
        readiness.has_value()) {
      score.surrogate_readiness_score +=
          std::clamp(*readiness, 0.0, 1.0) * 100.0;
    }
  }
  if (is_emulated_surrogate(result.mode_descriptor)) {
    ++score.emulated_surrogate_count;
  }

  if (performance_valid) {
    if (result.ber.has_value()) {
      ++score.ber_available_count;
    } else if (!result.mode_descriptor.supports_bit_payload) {
      ++score.audio_only_ber_fer_na_count;
    } else {
      ++score.ber_unavailable_count;
    }

    if (result.fer.has_value()) {
      ++score.fer_available_count;
    } else if (result.mode_descriptor.supports_bit_payload) {
      ++score.fer_unavailable_count;
    }
  }

  if (has_optional_slot(result, "asr_wer")) {
    ++score.asr_wer_slot_count;
  }
  if (has_optional_slot(result, "stoi")) {
    ++score.stoi_slot_count;
  }
  if (has_optional_slot(result, "estoi")) {
    ++score.estoi_slot_count;
  }
  if (has_optional_slot(result, "subjective_note")) {
    ++score.subjective_note_slot_count;
  }
}

void finalize_score(M2ModeScore &score, const M2ScorePolicy &policy) {
  if (score.record_count == 0) {
    score.score = 0.0;
    return;
  }

  const auto record_count = static_cast<double>(score.record_count);
  score.mean_dropout_rate /= record_count;
  score.mean_latency_s /= record_count;
  const auto valid_completed_count =
      score.completed_count > score.performance_invalid_count
          ? score.completed_count - score.performance_invalid_count
          : 0;
  score.completed_run_ratio =
      static_cast<double>(valid_completed_count) / record_count;

  const double failed_ratio = static_cast<double>(score.failed_count) / record_count;
  const double skipped_ratio =
      static_cast<double>(score.skipped_count) / record_count;

  score.skipped_penalty_total = skipped_ratio * policy.skipped_penalty;
  score.failed_penalty_total = failed_ratio * policy.failed_penalty;
  score.dropout_penalty =
      std::max(0.0, score.mean_dropout_rate) * policy.dropout_penalty_weight;
  score.latency_penalty =
      std::max(0.0, score.mean_latency_s - policy.latency_reference_s) *
      policy.latency_penalty_per_s;
  if (policy.target_rf_bandwidth_hz > 0.0) {
    score.bandwidth_penalty =
        std::max(0.0, score.max_rf_bandwidth_hz - policy.target_rf_bandwidth_hz) /
        policy.target_rf_bandwidth_hz *
        policy.bandwidth_penalty_per_target_ratio;
  }

  const double evidence_score =
      (static_cast<double>(score.performance_valid_count) / record_count) *
          policy.completed_run_weight +
      failed_ratio * policy.failed_run_evidence_weight;
  score.real_performance_score =
      clamp_score(evidence_score - score.skipped_penalty_total -
                  score.failed_penalty_total - score.dropout_penalty -
                  score.latency_penalty - score.bandwidth_penalty);
  score.score = score.real_performance_score;
  if (score.surrogate_count > 0) {
    score.surrogate_readiness_score /=
        static_cast<double>(score.surrogate_count);
  }
}

} // namespace

const M2ModeScore *M2ScoreReport::find_mode(
    const std::string &mode_id) const noexcept {
  const auto it = std::find_if(
      mode_scores.begin(), mode_scores.end(),
      [&mode_id](const M2ModeScore &score) { return score.mode_id == mode_id; });
  return it == mode_scores.end() ? nullptr : &*it;
}

M2ScorePolicy make_m2_score_policy() { return M2ScorePolicy{}; }

M2ScoreReport score_m2_results(
    const std::vector<ResultArtifact> &results,
    const std::vector<ModeDescriptorSnapshot> &profile_snapshots,
    const M2ScorePolicy &policy) {
  std::map<std::string, M2ModeScore> by_mode;

  for (const auto &snapshot : profile_snapshots) {
    auto &score = by_mode[snapshot.mode_id];
    score.mode_id = snapshot.mode_id;
    ensure_profile_snapshot(score, snapshot);
  }

  for (const auto &result : results) {
    const auto mode_id = result.mode_descriptor.mode_id;
    auto &score = by_mode[mode_id];
    score.mode_id = mode_id;
    add_result_to_score(score, result);
  }

  M2ScoreReport report;
  report.policy_id = policy.policy_id;
  report.total_records = results.size();
  report.mode_scores.reserve(by_mode.size());
  for (auto &[_, score] : by_mode) {
    finalize_score(score, policy);
    report.mode_scores.push_back(std::move(score));
  }
  std::sort(report.mode_scores.begin(), report.mode_scores.end(),
            [](const M2ModeScore &lhs, const M2ModeScore &rhs) {
              if (lhs.score != rhs.score) {
                return lhs.score > rhs.score;
              }
              return lhs.mode_id < rhs.mode_id;
            });
  return report;
}

std::string m2_score_report_to_json(const M2ScoreReport &report) {
  std::ostringstream out;
  out << "{";
  out << "\"policy_id\":\"" << escape_json(report.policy_id) << "\",";
  out << "\"total_records\":" << report.total_records << ",";
  out << "\"mode_scores\":[";
  for (std::size_t i = 0; i < report.mode_scores.size(); ++i) {
    const auto &score = report.mode_scores[i];
    if (i > 0) {
      out << ",";
    }
    out << "{";
    out << "\"mode_id\":\"" << escape_json(score.mode_id) << "\",";
    out << "\"score\":" << score.score << ",";
    out << "\"completed_run_ratio\":" << score.completed_run_ratio << ",";
    out << "\"completed_count\":" << score.completed_count << ",";
    out << "\"failed_count\":" << score.failed_count << ",";
    out << "\"skipped_count\":" << score.skipped_count << ",";
    out << "\"official_unavailable_count\":"
        << score.official_unavailable_count << ",";
    out << "\"profile_only_count\":" << score.profile_only_count << ",";
    out << "\"surrogate_count\":" << score.surrogate_count << ",";
    out << "\"emulated_surrogate_count\":" << score.emulated_surrogate_count
        << ",";
    out << "\"performance_valid_count\":" << score.performance_valid_count << ",";
    out << "\"performance_invalid_count\":" << score.performance_invalid_count
        << ",";
    out << "\"audio_only_ber_fer_na_count\":"
        << score.audio_only_ber_fer_na_count << ",";
    out << "\"ber_available_count\":" << score.ber_available_count << ",";
    out << "\"fer_available_count\":" << score.fer_available_count << ",";
    out << "\"ber_unavailable_count\":" << score.ber_unavailable_count << ",";
    out << "\"fer_unavailable_count\":" << score.fer_unavailable_count << ",";
    out << "\"asr_wer_slot_count\":" << score.asr_wer_slot_count << ",";
    out << "\"stoi_slot_count\":" << score.stoi_slot_count << ",";
    out << "\"estoi_slot_count\":" << score.estoi_slot_count << ",";
    out << "\"subjective_note_slot_count\":"
        << score.subjective_note_slot_count << ",";
    out << "\"mean_dropout_rate\":" << score.mean_dropout_rate << ",";
    out << "\"mean_latency_s\":" << score.mean_latency_s << ",";
    out << "\"max_rf_bandwidth_hz\":" << score.max_rf_bandwidth_hz << ",";
    out << "\"skipped_penalty_total\":" << score.skipped_penalty_total << ",";
    out << "\"failed_penalty_total\":" << score.failed_penalty_total << ",";
    out << "\"dropout_penalty\":" << score.dropout_penalty << ",";
    out << "\"latency_penalty\":" << score.latency_penalty << ",";
    out << "\"bandwidth_penalty\":" << score.bandwidth_penalty << ",";
    out << "\"real_performance_score\":" << score.real_performance_score << ",";
    out << "\"surrogate_readiness_score\":"
        << score.surrogate_readiness_score << ",";
    out << "\"profile_only\":" << (score.profile_only ? "true" : "false")
        << ",";
    out << "\"surrogate\":" << (score.surrogate ? "true" : "false");
    if (score.profile_snapshot.has_value()) {
      out << ",\"profile_snapshot\":{";
      out << "\"mode_id\":\"" << escape_json(score.profile_snapshot->mode_id)
          << "\",";
      out << "\"display_name\":\""
          << escape_json(score.profile_snapshot->display_name) << "\",";
      out << "\"rf_bandwidth_hz\":"
          << score.profile_snapshot->rf_bandwidth_hz << ",";
      out << "\"audio_bandwidth_hz\":"
          << score.profile_snapshot->audio_bandwidth_hz << ",";
      out << "\"official_baseline\":"
          << (score.profile_snapshot->official_baseline ? "true" : "false")
          << ",";
      out << "\"emulator\":"
          << (score.profile_snapshot->emulator ? "true" : "false") << ",";
      out << "\"implementation_status\":\""
          << escape_json(score.profile_snapshot->implementation_status) << "\",";
      out << "\"not_real_modem\":"
          << (score.profile_snapshot->not_real_modem ? "true" : "false")
          << ",";
      out << "\"downselect_valid\":"
          << (score.profile_snapshot->downselect_valid ? "true" : "false")
          << ",";
      out << "\"not_downselect_valid\":"
          << (score.profile_snapshot->not_downselect_valid ? "true" : "false")
          << ",";
      out << "\"performance_valid\":"
          << (score.profile_snapshot->performance_valid ? "true" : "false")
          << ",";
      out << "\"surrogate_model_name\":\""
          << escape_json(score.profile_snapshot->surrogate_model_name) << "\",";
      out << "\"surrogate_model_version\":\""
          << escape_json(score.profile_snapshot->surrogate_model_version)
          << "\",";
      out << "\"surrogate_limitations\":\""
          << escape_json(score.profile_snapshot->surrogate_limitations) << "\"";
      out << "}";
    }
    out << "}";
  }
  out << "]}";
  return out.str();
}

} // namespace f700f::metrics

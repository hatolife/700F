#pragma once

#include <f700f/metrics_artifacts.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace f700f::metrics {

struct M2ScorePolicy {
  std::string policy_id = "m2-interim-v0";
  double target_rf_bandwidth_hz = 1900.0;
  double latency_reference_s = 0.20;
  double completed_run_weight = 100.0;
  double failed_run_evidence_weight = 55.0;
  double skipped_penalty = 20.0;
  double failed_penalty = 20.0;
  double dropout_penalty_weight = 20.0;
  double latency_penalty_per_s = 20.0;
  double bandwidth_penalty_per_target_ratio = 15.0;
};

struct M2ModeScore {
  std::string mode_id;
  std::optional<ModeDescriptorSnapshot> profile_snapshot;
  std::uint64_t record_count = 0;
  std::uint64_t completed_count = 0;
  std::uint64_t failed_count = 0;
  std::uint64_t skipped_count = 0;
  std::uint64_t official_unavailable_count = 0;
  std::uint64_t profile_only_count = 0;
  std::uint64_t emulated_surrogate_count = 0;
  std::uint64_t performance_invalid_count = 0;
  std::uint64_t audio_only_ber_fer_na_count = 0;
  std::uint64_t ber_available_count = 0;
  std::uint64_t fer_available_count = 0;
  std::uint64_t ber_unavailable_count = 0;
  std::uint64_t fer_unavailable_count = 0;
  std::uint64_t asr_wer_slot_count = 0;
  std::uint64_t stoi_slot_count = 0;
  std::uint64_t estoi_slot_count = 0;
  std::uint64_t subjective_note_slot_count = 0;
  double completed_run_ratio = 0.0;
  double mean_dropout_rate = 0.0;
  double mean_latency_s = 0.0;
  double max_rf_bandwidth_hz = 0.0;
  double skipped_penalty_total = 0.0;
  double failed_penalty_total = 0.0;
  double dropout_penalty = 0.0;
  double latency_penalty = 0.0;
  double bandwidth_penalty = 0.0;
  double score = 0.0;
  bool profile_only = false;
};

struct M2ScoreReport {
  std::string policy_id;
  std::uint64_t total_records = 0;
  std::vector<M2ModeScore> mode_scores;

  const M2ModeScore *find_mode(const std::string &mode_id) const noexcept;
};

M2ScorePolicy make_m2_score_policy();

M2ScoreReport score_m2_results(
    const std::vector<ResultArtifact> &results,
    const std::vector<ModeDescriptorSnapshot> &profile_snapshots = {},
    const M2ScorePolicy &policy = make_m2_score_policy());

std::string m2_score_report_to_json(const M2ScoreReport &report);

} // namespace f700f::metrics

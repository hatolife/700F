#pragma once

#include <f700f/scoring_model.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace f700f::reporting {

struct M2BaselineComparisonReportContext {
  std::string run_id;
  std::string commit_hash;
  std::string config_path;
  std::string generated_at_utc;
  std::string sweep_status = "not provided";
  std::vector<std::string> mode_ids;
  std::vector<std::string> channel_ids;
  std::vector<std::uint64_t> seeds;
  std::vector<std::string> known_limitations;
  bool real_downselect_possible = false;
  std::string downselect_feasibility_summary =
      "Real downselect possible: no; complete waveform evidence is not available.";
  std::string next_recommendation_placeholder =
      "Pending M2 downselect review after complete sweep evidence.";
};

std::string render_m2_baseline_comparison_report(
    const M2BaselineComparisonReportContext &context,
    const f700f::metrics::M2ScoreReport &score_report);

std::string make_m2_baseline_report_filename(const std::string &run_id,
                                             const std::string &timestamp_utc);

} // namespace f700f::reporting

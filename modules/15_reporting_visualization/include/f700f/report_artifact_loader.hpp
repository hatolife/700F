#pragma once

#include <f700f/baseline_comparison_report.hpp>
#include <f700f/metrics_artifacts.hpp>

#include <string>
#include <vector>

namespace f700f::reporting {

struct LoadedReportInput {
  M2BaselineComparisonReportContext context;
  std::vector<f700f::metrics::ResultArtifact> results;
  std::vector<f700f::metrics::ModeDescriptorSnapshot> profile_snapshots;
  bool real_downselect_possible = false;
};

LoadedReportInput load_report_input_json(const std::string &json_payload);
LoadedReportInput load_report_input_csv(const std::string &csv_payload);
LoadedReportInput load_report_input_file(const std::string &path);

std::string render_report_from_loaded_input(const LoadedReportInput &loaded);

} // namespace f700f::reporting

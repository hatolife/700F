#include <f700f/baseline_comparison_report.hpp>
#include <f700f/candidate_profiles.hpp>
#include <f700f/metrics_artifacts.hpp>
#include <f700f/scoring_model.hpp>

#include <cassert>
#include <string>
#include <vector>

namespace {

bool contains(const std::string &haystack, const std::string &needle) {
  return haystack.find(needle) != std::string::npos;
}

f700f::metrics::ResultArtifact make_result(std::string mode_id) {
  f700f::ModeDescriptor descriptor;
  descriptor.mode_id = std::move(mode_id);
  descriptor.display_name = descriptor.mode_id;
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 1900.0;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3000.0;
  descriptor.audio_bandwidth_hz = 2700.0;
  descriptor.nominal_latency_s = 0.12;
  descriptor.codec_id = "analog-voice";
  descriptor.fec_id = "none";
  descriptor.modem_id = "ssb";
  descriptor.implementation_status = "emulated";
  descriptor.emulator = true;
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_output = true;

  f700f::metrics::ResultArtifact result;
  result.project_version = "v0.2.0";
  result.module_version = "v0.2.0";
  result.run_id = "m2-smoke-" + descriptor.mode_id;
  result.mode_descriptor = f700f::metrics::make_mode_descriptor_snapshot(descriptor);
  result.channel_id = "m2_awgn_6db";
  result.channel_parameters = {{"snr_db", "6.0"}};
  result.seed = 1;
  result.snr_db = 6.0;
  result.frame_count = 4;
  result.sample_count = 3200;
  result.dropout_rate = 0.125;
  result.latency_estimate_s = 0.12;
  result.optional_metrics["asr_wer"] = "N/A";
  return result;
}

f700f::reporting::M2BaselineComparisonReportContext make_context() {
  f700f::reporting::M2BaselineComparisonReportContext context;
  context.run_id = "m2-smoke";
  context.commit_hash = "abc1234";
  context.config_path = "configs/sweeps/m2_smoke.toml";
  context.generated_at_utc = "2026-06-28T12:34:56Z";
  context.mode_ids = {"ssb_standard_3k", "freedv700d_official",
                      "freedv700f_a_balanced"};
  context.channel_ids = {"m2_awgn_6db"};
  context.seeds = {1};
  context.known_limitations = {"ASR/STOI metrics are slot-only in ISSUE-0024."};
  context.next_recommendation_placeholder =
      "Pending human downselect after M2 sweep evidence.";
  return context;
}

void empty_report_is_deterministic_and_describes_missing_sweep() {
  auto context = make_context();
  context.sweep_status = "blocked: ISSUE-0026 sweep execution unavailable";
  const auto score_report = f700f::metrics::score_m2_results({});
  const auto markdown =
      f700f::reporting::render_m2_baseline_comparison_report(context, score_report);

  assert(contains(markdown, "# M2 Baseline Comparison Report"));
  assert(contains(markdown, "Run id: `m2-smoke`"));
  assert(contains(markdown, "Commit: `abc1234`"));
  assert(contains(markdown, "Config: `configs/sweeps/m2_smoke.toml`"));
  assert(contains(markdown,
                  "Sweep status: blocked: ISSUE-0026 sweep execution unavailable"));
  assert(contains(markdown, "No scored mode entries were available."));
  assert(contains(markdown, "Next recommendation placeholder"));
}

void markdown_includes_scores_statuses_and_raw_metric_summary() {
  auto completed = make_result("ssb_standard_3k");
  auto skipped = make_result("freedv700d_official");
  skipped.mode_descriptor.official_baseline = true;
  skipped.mode_descriptor.emulator = false;
  skipped.mode_descriptor.implementation_status = "unavailable";
  skipped.skipped_reason = "official adapter unavailable: Codec2 disabled";
  auto surrogate = make_result("freedv700d_emulated");
  surrogate.mode_descriptor.implementation_status = "emulated_surrogate";
  surrogate.mode_descriptor.official_baseline = false;
  surrogate.optional_metrics["not_official_freedv"] = "true";
  surrogate.optional_metrics["downselect_valid"] = "false";
  surrogate.optional_metrics["performance_valid"] = "false";
  surrogate.optional_metrics["emulator_limitations"] =
      "deterministic surrogate only, not official FreeDV performance";

  const auto profile = f700f::metrics::make_mode_descriptor_snapshot(
      f700f::freedv700f_a_balanced_descriptor());
  const auto score_report =
      f700f::metrics::score_m2_results({skipped, completed, surrogate}, {profile});
  const auto markdown = f700f::reporting::render_m2_baseline_comparison_report(
      make_context(), score_report);

  assert(contains(markdown, "## Mode List"));
  assert(contains(markdown, "`ssb_standard_3k`"));
  assert(contains(markdown, "## Channel List"));
  assert(contains(markdown, "`m2_awgn_6db`"));
  assert(contains(markdown, "## Seeds"));
  assert(contains(markdown, "`1`"));
  assert(contains(markdown, "| Mode | Score | Completed | Failed | Skipped |"));
  assert(contains(markdown, "`freedv700d_official`"));
  assert(contains(markdown, "official adapter unavailable"));
  assert(contains(markdown, "`freedv700d_emulated`"));
  assert(contains(markdown, "emulated_surrogate"));
  assert(contains(markdown, "performance invalid"));
  assert(contains(markdown, "not_official_freedv=true"));
  assert(contains(markdown, "Official FreeDV status"));
  assert(contains(markdown, "unavailable/skipped: 1"));
  assert(contains(markdown, "SSB reference status"));
  assert(contains(markdown, "700F candidate summary"));
  assert(contains(markdown, "`freedv700f_a_balanced` profile_only score 0"));
  assert(contains(markdown, "## Raw Metrics Summary"));
  assert(contains(markdown, "mean dropout"));
  assert(contains(markdown, "ASR WER slots"));
  assert(contains(markdown, "## Known Limitations"));
}

void timestamped_filename_is_supported() {
  const auto filename = f700f::reporting::make_m2_baseline_report_filename(
      "m2 smoke/run", "2026-06-28T12:34:56Z");
  assert(filename == "m2-baseline-comparison-m2_smoke_run-20260628T123456Z.md");
}

} // namespace

int main() {
  empty_report_is_deterministic_and_describes_missing_sweep();
  markdown_includes_scores_statuses_and_raw_metric_summary();
  timestamped_filename_is_supported();
  return 0;
}

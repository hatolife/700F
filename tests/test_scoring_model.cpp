#include <f700f/candidate_profiles.hpp>
#include <f700f/metrics_artifacts.hpp>
#include <f700f/scoring_model.hpp>

#include <cassert>
#include <string>
#include <vector>

namespace {

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
  descriptor.emulator = true;
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_output = true;

  f700f::metrics::ResultArtifact result;
  result.project_version = "v0.2.0";
  result.module_version = "v0.2.0";
  result.run_id = "run-" + descriptor.mode_id;
  result.mode_descriptor = f700f::metrics::make_mode_descriptor_snapshot(descriptor);
  result.channel_id = "awgn";
  result.channel_parameters = {{"snr_db", "12.0"}};
  result.seed = 7;
  result.snr_db = 12.0;
  result.frame_count = 10;
  result.sample_count = 8000;
  result.dropout_rate = 0.0;
  result.latency_estimate_s = 0.12;
  result.optional_metrics["asr_wer"] = "N/A";
  result.optional_metrics["stoi"] = "N/A";
  result.optional_metrics["estoi"] = "N/A";
  result.optional_metrics["subjective_note"] = "";
  return result;
}

void empty_results_are_deterministic() {
  const auto report =
      f700f::metrics::score_m2_results({}, {}, f700f::metrics::make_m2_score_policy());
  assert(report.total_records == 0);
  assert(report.mode_scores.empty());
  assert(report.policy_id == "m2-interim-v0");
}

void completed_failed_and_skipped_records_are_counted_and_ordered() {
  auto completed = make_result("ssb_standard_3k");
  auto failed = make_result("freedv700d_emulated");
  failed.error_summary = "decode failed";
  failed.dropout_rate = 0.4;
  failed.latency_estimate_s = 0.45;
  failed.mode_descriptor.rf_bandwidth_hz = 2600.0;

  auto skipped = make_result("freedv700e_official");
  skipped.mode_descriptor.official_baseline = true;
  skipped.mode_descriptor.emulator = false;
  skipped.skipped_reason = "official adapter unavailable: Codec2 disabled";

  const auto report = f700f::metrics::score_m2_results(
      {failed, skipped, completed}, {}, f700f::metrics::make_m2_score_policy());

  const auto *ssb = report.find_mode("ssb_standard_3k");
  const auto *freedv_failed = report.find_mode("freedv700d_emulated");
  const auto *official = report.find_mode("freedv700e_official");
  assert(ssb != nullptr);
  assert(freedv_failed != nullptr);
  assert(official != nullptr);
  assert(ssb->completed_count == 1);
  assert(ssb->completed_run_ratio == 1.0);
  assert(freedv_failed->failed_count == 1);
  assert(freedv_failed->dropout_penalty > 0.0);
  assert(freedv_failed->latency_penalty > 0.0);
  assert(freedv_failed->bandwidth_penalty > 0.0);
  assert(official->skipped_count == 1);
  assert(official->official_unavailable_count == 1);
  assert(ssb->score > freedv_failed->score);
  assert(freedv_failed->score > official->score);
}

void audio_only_na_and_digital_availability_are_represented() {
  auto audio_only = make_result("ssb_narrow_1k9");
  audio_only.ber.reset();
  audio_only.fer.reset();
  audio_only.mode_descriptor.supports_bit_payload = false;

  auto digital = make_result("freedv700d_emulated");
  digital.mode_descriptor.supports_bit_payload = true;
  digital.ber = 0.01;
  digital.fer = 0.02;

  const auto report = f700f::metrics::score_m2_results(
      {audio_only, digital}, {}, f700f::metrics::make_m2_score_policy());

  const auto *audio_score = report.find_mode("ssb_narrow_1k9");
  const auto *digital_score = report.find_mode("freedv700d_emulated");
  assert(audio_score != nullptr);
  assert(digital_score != nullptr);
  assert(audio_score->audio_only_ber_fer_na_count == 1);
  assert(audio_score->ber_available_count == 0);
  assert(digital_score->ber_available_count == 1);
  assert(digital_score->fer_available_count == 1);
}

void surrogate_snapshot_is_carried_without_performance_evidence() {
  const auto surrogate = f700f::metrics::make_mode_descriptor_snapshot(
      f700f::freedv700f_b_robust_descriptor());

  const auto report = f700f::metrics::score_m2_results(
      {}, {surrogate}, f700f::metrics::make_m2_score_policy());

  const auto *candidate = report.find_mode("freedv700f_b_robust");
  assert(candidate != nullptr);
  assert(candidate->surrogate);
  assert(candidate->completed_count == 0);
  assert(candidate->score == 0.0);
  assert(candidate->real_performance_score == 0.0);
  assert(candidate->surrogate_readiness_score == 0.0);
  assert(candidate->profile_snapshot.has_value());
  assert(candidate->profile_snapshot->implementation_status == "surrogate");
  assert(candidate->profile_snapshot->not_real_modem);
  assert(!candidate->profile_snapshot->downselect_valid);
  assert(candidate->profile_snapshot->not_downselect_valid);
  assert(!candidate->profile_snapshot->performance_valid);
  assert(candidate->profile_snapshot->rf_bandwidth_hz == 1900.0);
}

void completed_surrogate_rows_do_not_score_as_real_performance() {
  auto surrogate = make_result("freedv700f_b_robust");
  surrogate.mode_descriptor = f700f::metrics::make_mode_descriptor_snapshot(
      f700f::freedv700f_b_robust_descriptor());
  surrogate.optional_metrics["surrogate_readiness_score_synthetic"] = "0.625";
  surrogate.optional_metrics["synthetic_metrics_label"] =
      "synthetic_surrogate_readiness_only";

  const auto report = f700f::metrics::score_m2_results(
      {surrogate}, {}, f700f::metrics::make_m2_score_policy());

  const auto *candidate = report.find_mode("freedv700f_b_robust");
  assert(candidate != nullptr);
  assert(candidate->completed_count == 1);
  assert(candidate->surrogate_count == 1);
  assert(candidate->performance_valid_count == 0);
  assert(candidate->performance_invalid_count == 1);
  assert(candidate->score == 0.0);
  assert(candidate->real_performance_score == 0.0);
  assert(candidate->surrogate_readiness_score > 62.0);
  assert(candidate->surrogate_readiness_score < 63.0);
  assert(candidate->ber_available_count == 0);
  assert(candidate->fer_available_count == 0);
  assert(candidate->ber_unavailable_count == 0);
  assert(candidate->fer_unavailable_count == 0);
}

void completed_waveform_prototype_rows_do_not_score_as_real_performance() {
  auto prototype = make_result("freedv700f_a_balanced");
  prototype.mode_descriptor = f700f::metrics::make_mode_descriptor_snapshot(
      f700f::freedv700f_a_balanced_descriptor());
  prototype.optional_metrics["prototype"] = "true";
  prototype.optional_metrics["not_final_modem"] = "true";
  prototype.optional_metrics["waveform_capable"] = "true";
  prototype.optional_metrics["downselect_valid"] = "false";
  prototype.optional_metrics["performance_valid"] = "false";

  const auto report = f700f::metrics::score_m2_results(
      {prototype}, {}, f700f::metrics::make_m2_score_policy());

  const auto *candidate = report.find_mode("freedv700f_a_balanced");
  assert(candidate != nullptr);
  assert(candidate->completed_count == 1);
  assert(candidate->surrogate_count == 0);
  assert(candidate->performance_valid_count == 0);
  assert(candidate->performance_invalid_count == 1);
  assert(candidate->score == 0.0);
  assert(candidate->real_performance_score == 0.0);
  assert(candidate->profile_snapshot.has_value());
  assert(candidate->profile_snapshot->implementation_status ==
         "waveform_prototype");
  assert(candidate->profile_snapshot->prototype);
  assert(candidate->profile_snapshot->not_final_modem);
  assert(candidate->profile_snapshot->waveform_capable);
  assert(!candidate->profile_snapshot->downselect_valid);
  assert(!candidate->profile_snapshot->performance_valid);
}

void emulated_surrogate_completed_rows_are_not_performance_evidence() {
  auto surrogate = make_result("freedv700d_emulated");
  surrogate.mode_descriptor.implementation_status = "emulated_surrogate";
  surrogate.mode_descriptor.official_baseline = false;
  surrogate.mode_descriptor.emulator = true;
  surrogate.optional_metrics["official"] = "false";
  surrogate.optional_metrics["not_official_freedv"] = "true";
  surrogate.optional_metrics["downselect_valid"] = "false";
  surrogate.optional_metrics["performance_valid"] = "false";
  surrogate.optional_metrics["emulator_limitations"] =
      "deterministic surrogate only, not official FreeDV performance";

  const auto report = f700f::metrics::score_m2_results(
      {surrogate}, {}, f700f::metrics::make_m2_score_policy());
  const auto *score = report.find_mode("freedv700d_emulated");
  assert(score != nullptr);
  assert(score->completed_count == 1);
  assert(score->emulated_surrogate_count == 1);
  assert(score->performance_invalid_count == 1);
  assert(score->completed_run_ratio == 0.0);
  assert(score->score == 0.0);
}

} // namespace

int main() {
  empty_results_are_deterministic();
  completed_failed_and_skipped_records_are_counted_and_ordered();
  audio_only_na_and_digital_availability_are_represented();
  surrogate_snapshot_is_carried_without_performance_evidence();
  completed_surrogate_rows_do_not_score_as_real_performance();
  completed_waveform_prototype_rows_do_not_score_as_real_performance();
  emulated_surrogate_completed_rows_are_not_performance_evidence();
  return 0;
}

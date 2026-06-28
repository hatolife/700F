#include <f700f/report_artifact_loader.hpp>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

bool contains(const std::string &haystack, const std::string &needle) {
  return haystack.find(needle) != std::string::npos;
}

void write_file(const std::filesystem::path &path, const std::string &text) {
  std::filesystem::create_directories(path.parent_path());
  std::ofstream out(path);
  out << text;
}

std::string sample_sweep_json() {
  return R"json({
  "ok": true,
  "run_id_prefix": "m2-700f-candidate-smoke",
  "error": "",
  "records": [
    {"run_id": "m2-ssb", "status": "completed", "mode_id": "ssb_standard_3k", "condition_id": "identity", "seed": 1, "skipped_reason": null, "error_summary": null, "simulation_digest": "abc"},
    {"run_id": "m2-official", "status": "skipped", "mode_id": "freedv700d_official", "condition_id": "identity", "seed": 1, "skipped_reason": "official_freedv_codec2_unavailable: F700F_ENABLE_CODEC2=OFF; official=true codec2_enabled=false codec2_available=false roundtrip_available=false not_emulator=true not_surrogate=true", "error_summary": null, "simulation_digest": ""},
    {"run_id": "m2-surrogate", "status": "completed", "mode_id": "freedv700f_b_robust", "condition_id": "awgn-snr-6db", "seed": 1, "skipped_reason": null, "error_summary": "surrogate_completed: not_real_modem=true downselect_valid=false performance_valid=false", "simulation_digest": "surrogate", "implementation_status": "surrogate", "not_real_modem": true, "downselect_valid": false, "not_downselect_valid": true, "performance_valid": false, "surrogate_model_name": "700f_candidate_minimal_behavior", "surrogate_model_version": "ISSUE-0032-v1", "surrogate_limitations": "synthetic readiness only; not a real modem; BER/FER are not emitted as real values", "surrogate_readiness_score_synthetic": "0.625", "synthetic_metrics_label": "synthetic_surrogate_readiness_only"},
    {"run_id": "m2-emulated", "status": "completed", "mode_id": "freedv700d_emulated", "condition_id": "awgn-snr-6db", "seed": 1, "skipped_reason": null, "error_summary": "emulated_surrogate_completed: implementation_status=emulated_surrogate; official=false; not_official_freedv=true; downselect_valid=false; performance_valid=false; emulator_model_name=f700f-minimal-freedv700d700e-surrogate; emulator_model_version=ISSUE-0033-v1; emulator_limitations=deterministic surrogate only, not official FreeDV performance", "simulation_digest": "emulated"}
  ]
})json";
}

std::string sample_sweep_csv() {
  return "run_id,status,mode_id,condition_id,seed,simulation_ok,digest,skipped_reason,error_summary,implementation_status,not_real_modem,downselect_valid,not_downselect_valid,performance_valid,surrogate_model_name,surrogate_model_version,surrogate_limitations,surrogate_readiness_score_synthetic,synthetic_metrics_label\n"
         "m2-ssb,completed,ssb_standard_3k,identity,1,true,abc,,,,,,,,,,,,\n"
         "m2-official,skipped,freedv700d_official,identity,1,false,,official_freedv_codec2_unavailable: F700F_ENABLE_CODEC2=OFF; official=true codec2_enabled=false codec2_available=false roundtrip_available=false not_emulator=true not_surrogate=true,,,,,,,,,,,\n"
         "m2-surrogate,completed,freedv700f_b_robust,awgn-snr-6db,1,true,surrogate,,surrogate_completed: not_real_modem=true downselect_valid=false performance_valid=false,surrogate,true,false,true,false,700f_candidate_minimal_behavior,ISSUE-0032-v1,synthetic readiness only not a real modem and not real BER/FER,0.625,synthetic_surrogate_readiness_only\n"
         "m2-emulated,completed,freedv700d_emulated,awgn-snr-6db,1,true,emulated,,emulated_surrogate_completed: implementation_status=emulated_surrogate; official=false; not_official_freedv=true; downselect_valid=false; performance_valid=false; emulator_model_name=f700f-minimal-freedv700d700e-surrogate; emulator_model_version=ISSUE-0033-v1; emulator_limitations=deterministic surrogate only not official FreeDV performance,,,,,,,,,,\n";
}

void sample_sweep_json_generates_markdown_with_all_row_types() {
  const auto loaded = f700f::reporting::load_report_input_json(sample_sweep_json());
  assert(loaded.results.size() == 4);
  assert(!loaded.real_downselect_possible);
  assert(loaded.results[2].mode_descriptor.implementation_status == "surrogate");
  assert(loaded.results[2].mode_descriptor.not_real_modem);
  assert(!loaded.results[2].mode_descriptor.downselect_valid);
  assert(loaded.results[2].mode_descriptor.not_downselect_valid);
  assert(!loaded.results[2].mode_descriptor.performance_valid);
  assert(loaded.results[2].optional_metrics.at(
             "surrogate_readiness_score_synthetic") == "0.625");
  assert(loaded.results[2].optional_metrics.at("synthetic_metrics_label")
             .find("synthetic") != std::string::npos);
  const auto &emulated = loaded.results.back();
  assert(emulated.mode_descriptor.mode_id == "freedv700d_emulated");
  assert(emulated.mode_descriptor.implementation_status == "emulated_surrogate");
  assert(!emulated.mode_descriptor.official_baseline);
  assert(emulated.mode_descriptor.emulator);
  assert(emulated.optional_metrics.at("official") == "false");
  assert(emulated.optional_metrics.at("not_official_freedv") == "true");
  assert(emulated.optional_metrics.at("downselect_valid") == "false");
  assert(emulated.optional_metrics.at("performance_valid") == "false");
  assert(emulated.optional_metrics.at("emulator_model_name") ==
         "f700f-minimal-freedv700d700e-surrogate");
  assert(contains(emulated.optional_metrics.at("emulator_limitations"),
                  "not official FreeDV performance"));
  assert(!emulated.warnings.empty());
  assert(contains(emulated.warnings.front(), "not_official_freedv=true"));

  const auto markdown = f700f::reporting::render_report_from_loaded_input(loaded);
  assert(contains(markdown, "# M2 Baseline Comparison Report"));
  assert(contains(markdown, "`freedv700d_official`"));
  assert(contains(markdown, "official_freedv_codec2_unavailable"));
  assert(contains(markdown, "`freedv700f_b_robust`"));
  assert(contains(markdown, "surrogate"));
  assert(contains(markdown, "SURROGATE WARNING"));
  assert(contains(markdown, "not_real_modem=true"));
  assert(contains(markdown, "downselect_valid=false"));
  assert(contains(markdown, "performance_valid=false"));
  assert(contains(markdown, "synthetic_surrogate_readiness_only"));
  assert(contains(markdown, "`freedv700d_emulated`"));
  assert(contains(markdown, "emulated_surrogate"));
  assert(contains(markdown, "not_official_freedv=true"));
  assert(contains(markdown, "performance_valid=false"));
  assert(contains(markdown, "emulator_limitations"));
  assert(contains(markdown, "Real downselect possible: no"));
}

void sample_sweep_csv_generates_markdown() {
  const auto loaded = f700f::reporting::load_report_input_csv(sample_sweep_csv());
  assert(loaded.results.size() == 4);
  assert(loaded.results[2].mode_descriptor.implementation_status == "surrogate");
  assert(loaded.results[2].mode_descriptor.not_real_modem);
  assert(!loaded.results[2].mode_descriptor.downselect_valid);
  assert(!loaded.results[2].mode_descriptor.performance_valid);
  const auto markdown = f700f::reporting::render_report_from_loaded_input(loaded);
  assert(contains(markdown, "`ssb_standard_3k`"));
  assert(contains(markdown, "`freedv700f_b_robust`"));
  assert(contains(markdown, "surrogate"));
  assert(contains(markdown, "Real downselect possible: no"));
}

void real_modem_prototype_json_is_loaded_as_limited_diagnostics() {
  const std::string json = R"json({
  "ok": true,
  "run_id_prefix": "issue0044-real-prototype",
  "error": "",
  "records": [
    {"run_id": "proto-a", "status": "completed", "mode_id": "freedv700f_a_balanced", "condition_id": "awgn-snr-6db", "seed": 7, "skipped_reason": null, "error_summary": "real_modem_prototype_completed: limited diagnostics only; performance_valid=limited", "simulation_digest": "proto", "implementation_status": "real_modem_prototype", "implementation_classification": "real_modem_prototype", "prototype": true, "not_final_modem": true, "waveform_capable": true, "downselect_valid": false, "not_downselect_valid": true, "performance_valid": false, "performance_validity": "limited", "downselect_validity": "invalid", "codec_family": "synthetic", "fec_family": "none", "sync_family": "none", "modem_family": "minimal_qpsk", "prototype_limitations": "diagnostics only", "prototype_warning": "REAL MODEM PROTOTYPE WARNING: limited diagnostics only; performance_valid=limited", "prototype_symbol_error_rate": 0.25, "prototype_frame_status": "completed", "prototype_sync_status": "none", "prototype_baseband_sample_count": 3840}
  ]
})json";

  const auto loaded = f700f::reporting::load_report_input_json(json);
  assert(loaded.results.size() == 1);
  assert(!loaded.real_downselect_possible);
  const auto &prototype = loaded.results.front();
  assert(prototype.mode_descriptor.implementation_status ==
         "real_modem_prototype");
  assert(prototype.mode_descriptor.implementation_classification ==
         "real_modem_prototype");
  assert(prototype.mode_descriptor.performance_validity == "limited");
  assert(prototype.mode_descriptor.downselect_validity == "invalid");
  assert(!prototype.mode_descriptor.downselect_valid);
  assert(prototype.mode_descriptor.sync_family == "none");
  assert(prototype.mode_descriptor.modem_family == "minimal_qpsk");
  assert(prototype.prototype_symbol_error_rate.has_value());
  assert(prototype.prototype_symbol_error_rate.value() == 0.25);
  assert(prototype.prototype_frame_status == "completed");
  assert(prototype.prototype_sync_status == "none");
  assert(prototype.prototype_baseband_sample_count == 3840);

  const auto markdown = f700f::reporting::render_report_from_loaded_input(loaded);
  assert(contains(markdown, "REAL MODEM PROTOTYPE WARNING"));
  assert(contains(markdown, "performance_valid=limited"));
  assert(contains(markdown, "performance_validity=limited"));
  assert(contains(markdown, "sync_family=none"));
  assert(contains(markdown, "prototype_symbol_error_rate=0.25"));
  assert(contains(markdown, "Real downselect possible: no"));
}

void malformed_inputs_fail_clearly() {
  try {
    (void)f700f::reporting::load_report_input_json("{ not json");
    assert(false);
  } catch (const std::exception &ex) {
    assert(contains(ex.what(), "Malformed JSON"));
  }

  try {
    (void)f700f::reporting::load_report_input_csv("run_id,status\nonly-one-field\n");
    assert(false);
  } catch (const std::exception &ex) {
    assert(contains(ex.what(), "Malformed CSV"));
  }
}

void file_loader_uses_extension() {
  const auto path = std::filesystem::path("build/test-artifacts/report-loader/sample.json");
  write_file(path, sample_sweep_json());
  const auto loaded = f700f::reporting::load_report_input_file(path.string());
  assert(loaded.context.run_id == "m2-700f-candidate-smoke");
  assert(loaded.results.size() == 4);
}

} // namespace

int main() {
  sample_sweep_json_generates_markdown_with_all_row_types();
  sample_sweep_csv_generates_markdown();
  real_modem_prototype_json_is_loaded_as_limited_diagnostics();
  malformed_inputs_fail_clearly();
  file_loader_uses_extension();
  return 0;
}

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
    {"run_id": "m2-official", "status": "skipped", "mode_id": "freedv700d_official", "condition_id": "identity", "seed": 1, "skipped_reason": "official_waveform_roundtrip_not_implemented", "error_summary": null, "simulation_digest": ""},
    {"run_id": "m2-profile", "status": "completed", "mode_id": "freedv700f_a_balanced", "condition_id": "awgn-snr-6db", "seed": 1, "skipped_reason": null, "error_summary": "profile_only_completed", "simulation_digest": "profile"},
    {"run_id": "m2-surrogate", "status": "completed", "mode_id": "freedv700d_emulated", "condition_id": "awgn-snr-6db", "seed": 1, "skipped_reason": null, "error_summary": "emulated_surrogate_completed: implementation_status=emulated_surrogate; official=false; not_official_freedv=true; downselect_valid=false; performance_valid=false; emulator_model_name=f700f-minimal-freedv700d700e-surrogate; emulator_model_version=ISSUE-0033-v1; emulator_limitations=deterministic surrogate only, not official FreeDV performance", "simulation_digest": "surrogate"}
  ]
})json";
}

std::string sample_sweep_csv() {
  return "run_id,status,mode_id,condition_id,seed,simulation_ok,digest,skipped_reason,error_summary\n"
         "m2-ssb,completed,ssb_standard_3k,identity,1,true,abc,,\n"
         "m2-official,skipped,freedv700d_official,identity,1,false,,official_waveform_roundtrip_not_implemented,\n"
         "m2-profile,completed,freedv700f_a_balanced,awgn-snr-6db,1,true,profile,,profile_only_completed\n"
         "m2-surrogate,completed,freedv700d_emulated,awgn-snr-6db,1,true,surrogate,,emulated_surrogate_completed: implementation_status=emulated_surrogate; official=false; not_official_freedv=true; downselect_valid=false; performance_valid=false; emulator_model_name=f700f-minimal-freedv700d700e-surrogate; emulator_model_version=ISSUE-0033-v1; emulator_limitations=deterministic surrogate only not official FreeDV performance\n";
}

void sample_sweep_json_generates_markdown_with_all_row_types() {
  const auto loaded = f700f::reporting::load_report_input_json(sample_sweep_json());
  assert(loaded.results.size() == 4);
  assert(!loaded.real_downselect_possible);
  const auto &surrogate = loaded.results.back();
  assert(surrogate.mode_descriptor.mode_id == "freedv700d_emulated");
  assert(surrogate.mode_descriptor.implementation_status == "emulated_surrogate");
  assert(!surrogate.mode_descriptor.official_baseline);
  assert(surrogate.mode_descriptor.emulator);
  assert(surrogate.optional_metrics.at("official") == "false");
  assert(surrogate.optional_metrics.at("not_official_freedv") == "true");
  assert(surrogate.optional_metrics.at("downselect_valid") == "false");
  assert(surrogate.optional_metrics.at("performance_valid") == "false");
  assert(surrogate.optional_metrics.at("emulator_model_name") ==
         "f700f-minimal-freedv700d700e-surrogate");
  assert(contains(surrogate.optional_metrics.at("emulator_limitations"),
                  "not official FreeDV performance"));
  assert(!surrogate.warnings.empty());
  assert(contains(surrogate.warnings.front(), "not_official_freedv=true"));

  const auto markdown = f700f::reporting::render_report_from_loaded_input(loaded);
  assert(contains(markdown, "# M2 Baseline Comparison Report"));
  assert(contains(markdown, "`freedv700d_official`"));
  assert(contains(markdown, "official_waveform_roundtrip_not_implemented"));
  assert(contains(markdown, "`freedv700f_a_balanced`"));
  assert(contains(markdown, "profile_only"));
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
  const auto markdown = f700f::reporting::render_report_from_loaded_input(loaded);
  assert(contains(markdown, "`ssb_standard_3k`"));
  assert(contains(markdown, "`freedv700f_a_balanced`"));
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
  malformed_inputs_fail_clearly();
  file_loader_uses_extension();
  return 0;
}

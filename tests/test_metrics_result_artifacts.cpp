#include <f700f/metrics_artifacts.hpp>
#include <f700f/types.hpp>

#include <cassert>
#include <string>
#include <vector>

using f700f::metrics::ResultArtifact;

namespace {

ResultArtifact ssb_like_result() {
  f700f::ModeDescriptor descriptor;
  descriptor.mode_id = f700f::ModeId{"ssb.reference"};
  descriptor.display_name = "SSB-like reference";
  descriptor.sample_rate_hz = 8000;
  descriptor.rf_bandwidth_hz = 3000.0;
  descriptor.audio_low_hz = 300.0;
  descriptor.audio_high_hz = 3000.0;
  descriptor.nominal_latency_s = 0.0;
  descriptor.codec_id = "analog-voice";
  descriptor.fec_id = "none";
  descriptor.modem_id = "ssb";
  descriptor.capabilities.audio_input = true;
  descriptor.capabilities.audio_output = true;
  descriptor.capabilities.complex_output = true;

  ResultArtifact result;
  result.project_version = "v0.1.0";
  result.module_version = "v0.1.0";
  result.run_id = "run-ssb-001";
  result.mode_descriptor = f700f::metrics::make_mode_descriptor_snapshot(descriptor);
  result.channel_id = "awgn";
  result.channel_parameters = {{"snr_db", "12.0"}, {"fading", "none"}};
  result.seed = 1234;
  result.snr_db = 12.0;
  result.freq_offset_hz = 0.0;
  result.frame_count = 16;
  result.sample_count = 128000;
  result.dropout_rate = 0.0;
  result.sync_loss_count = 0;
  result.latency_estimate_s = 0.1;
  result.audio_export_path = "/tmp/ssb.wav";
  return result;
}

ResultArtifact digital_like_result() {
  ResultArtifact result = ssb_like_result();
  result.mode_descriptor = f700f::metrics::ModeDescriptorSnapshot{
      .mode_id = "freedv.700d",
      .display_name = "FreeDV-like 700D",
      .sample_rate_hz = 8000,
      .rf_bandwidth_hz = 1250.0,
      .audio_low_hz = 300.0,
      .audio_high_hz = 3000.0,
      .nominal_latency_s = 0.16,
      .codec_id = "codec2",
      .fec_id = "ldpc",
      .modem_id = "ofdm",
      .supports_audio_input = true,
      .supports_audio_output = true,
      .supports_complex_input = true,
      .supports_complex_output = true,
      .supports_bit_payload = true};
  result.channel_id = "fading-chan";
  result.channel_parameters = {{"snr_db", "6.5"}, {"freq_offset_hz", "300"}};
  result.seed = 5678;
  result.snr_db = 6.5;
  result.freq_offset_hz = 300.0;
  result.ber = 0.012;
  result.fer = 0.03;
  result.frame_count = 200;
  result.sample_count = 240000;
  result.dropout_rate = 0.05;
  result.sync_loss_count = 3;
  result.latency_estimate_s = 0.21;
  result.audio_export_path = "/tmp/freedv.wav";
  return result;
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
  fields.push_back(current);
  return fields;
}

void test_empty_result_object() {
  const auto artifact = f700f::metrics::make_empty_result_artifact();
  const auto json = f700f::metrics::to_json(artifact);
  const auto parsed = f700f::metrics::from_json(json);
  assert(parsed.project_version.empty());
  assert(parsed.mode_descriptor.mode_id.empty());
}

void test_ssb_result_uses_na_for_bit_metrics() {
  auto artifact = ssb_like_result();
  const auto json = f700f::metrics::to_json(artifact);
  assert(json.find("\"ber\":\"N/A\"") != std::string::npos);
  assert(json.find("\"fer\":\"N/A\"") != std::string::npos);
  const auto parsed = f700f::metrics::from_json(json);
  assert(!parsed.ber.has_value());
  assert(!parsed.fer.has_value());
}

void test_digital_result_records_bit_metrics() {
  auto artifact = digital_like_result();
  const auto json = f700f::metrics::to_json(artifact);
  assert(json.find("\"ber\":0.012") != std::string::npos);
  assert(json.find("\"fer\":0.03") != std::string::npos);
  const auto parsed = f700f::metrics::from_json(json);
  assert(parsed.ber.has_value() && parsed.ber.value() == artifact.ber.value());
  assert(parsed.fer.has_value() && parsed.fer.value() == artifact.fer.value());
}

void test_skipped_result() {
  auto artifact = ssb_like_result();
  artifact.skipped_reason = "mode not supported for this sweep";
  artifact.warnings = {"missing reference audio", "fallback to skip"};
  const auto json = f700f::metrics::to_json(artifact);
  const auto parsed = f700f::metrics::from_json(json);
  assert(parsed.skipped_reason.has_value());
  assert(*parsed.skipped_reason == artifact.skipped_reason);
  assert(parsed.warnings.size() == artifact.warnings.size());
}

void test_json_is_stable() {
  const auto artifact = digital_like_result();
  const auto json = f700f::metrics::to_json(artifact);
  std::vector<std::string> ordered_fields = {
      "\"project_version\"", "\"module_version\"", "\"run_id\"",
      "\"mode_descriptor\"", "\"mode_id\"", "\"channel_id\"",
      "\"channel_parameters\"", "\"seed\"",       "\"snr_db\"",
      "\"freq_offset_hz\"", "\"frame_count\"", "\"sample_count\"",
      "\"ber\"",            "\"fer\"",          "\"sync_loss_count\"",
      "\"dropout_rate\"",   "\"latency_estimate_s\"",
      "\"audio_export_path\"", "\"warnings\"", "\"skipped_reason\"",
      "\"error_summary\"", "\"optional_metrics\""};
  std::size_t cursor = 0;
  for (const auto &field : ordered_fields) {
    const auto pos = json.find(field, cursor);
    assert(pos != std::string::npos);
    assert(pos >= cursor);
    cursor = pos + field.size();
  }
}

void test_csv_minimum_columns() {
  const auto artifact = ssb_like_result();
  const auto header = f700f::metrics::to_csv_header({});
  const auto row = f700f::metrics::to_csv_row(artifact, {});
  const std::vector<std::string> expected_columns = {
      "project_version", "module_version", "run_id", "mode_id",
      "mode_descriptor_snapshot", "channel_id", "channel_parameters", "seed",
      "snr_db", "freq_offset_hz", "frame_count", "sample_count", "ber", "fer",
      "sync_loss_count", "dropout_rate", "latency_estimate_s",
      "audio_export_path", "warnings", "skipped_reason", "error_summary"};
  const auto parsed_header = split_csv_line(header);
  for (std::size_t i = 0; i < expected_columns.size(); ++i) {
    assert(i < parsed_header.size());
    assert(parsed_header[i] == expected_columns[i]);
  }
  assert(!row.empty());
}

void test_unknown_optional_metrics_survive() {
  auto artifact = digital_like_result();
  artifact.optional_metrics["asr_wer"] = "0.23";
  artifact.optional_metrics["stoi"] = "0.76";
  const auto extras = f700f::metrics::metric_column_names(artifact);
  const auto json = f700f::metrics::to_json(artifact);
  auto parsed = f700f::metrics::from_json(json);
  assert(parsed.optional_metrics.at("asr_wer") == "0.23");
  assert(parsed.optional_metrics.at("stoi") == "0.76");
  const auto header = f700f::metrics::to_csv_header(extras);
  const auto row = f700f::metrics::to_csv_row(artifact, extras);
  parsed = f700f::metrics::from_csv_row(header, row);
  assert(parsed.optional_metrics.at("asr_wer") == "0.23");
  assert(parsed.optional_metrics.at("stoi") == "0.76");
}

void test_surrogate_guardrails_survive_result_serialization() {
  auto artifact = digital_like_result();
  artifact.mode_descriptor.implementation_status = "surrogate";
  artifact.mode_descriptor.not_real_modem = true;
  artifact.mode_descriptor.downselect_valid = false;
  artifact.mode_descriptor.not_downselect_valid = true;
  artifact.mode_descriptor.performance_valid = false;
  artifact.mode_descriptor.surrogate_model_name =
      "700f_candidate_minimal_behavior";
  artifact.mode_descriptor.surrogate_model_version = "ISSUE-0032-v1";
  artifact.mode_descriptor.surrogate_limitations =
      "synthetic readiness only; not a real modem; BER/FER are not emitted as real values";
  artifact.ber.reset();
  artifact.fer.reset();
  artifact.optional_metrics["surrogate_readiness_score_synthetic"] = "0.625";
  artifact.optional_metrics["synthetic_metrics_label"] =
      "synthetic_surrogate_readiness_only";

  const auto json = f700f::metrics::to_json(artifact);
  assert(json.find("\"implementation_status\":\"surrogate\"") !=
         std::string::npos);
  assert(json.find("\"not_real_modem\":true") != std::string::npos);
  assert(json.find("\"downselect_valid\":false") != std::string::npos);
  assert(json.find("\"not_downselect_valid\":true") != std::string::npos);
  assert(json.find("\"performance_valid\":false") != std::string::npos);
  assert(json.find("surrogate_readiness_score_synthetic") != std::string::npos);
  auto parsed = f700f::metrics::from_json(json);
  assert(parsed.mode_descriptor.implementation_status == "surrogate");
  assert(parsed.mode_descriptor.not_real_modem);
  assert(!parsed.mode_descriptor.downselect_valid);
  assert(parsed.mode_descriptor.not_downselect_valid);
  assert(!parsed.mode_descriptor.performance_valid);
  assert(parsed.mode_descriptor.surrogate_model_name ==
         "700f_candidate_minimal_behavior");
  assert(parsed.optional_metrics.at("synthetic_metrics_label") ==
         "synthetic_surrogate_readiness_only");

  const auto extras = f700f::metrics::metric_column_names(artifact);
  const auto header = f700f::metrics::to_csv_header(extras);
  const auto row = f700f::metrics::to_csv_row(artifact, extras);
  parsed = f700f::metrics::from_csv_row(header, row);
  assert(parsed.mode_descriptor.implementation_status == "surrogate");
  assert(parsed.mode_descriptor.not_real_modem);
  assert(!parsed.mode_descriptor.downselect_valid);
  assert(!parsed.mode_descriptor.performance_valid);
  assert(parsed.optional_metrics.at("surrogate_readiness_score_synthetic") ==
         "0.625");
}

void test_real_modem_prototype_diagnostics_survive_result_serialization() {
  auto artifact = digital_like_result();
  artifact.mode_descriptor.mode_id = "freedv700f_a_balanced";
  artifact.mode_descriptor.implementation_status = "real_modem_prototype";
  artifact.mode_descriptor.implementation_classification =
      "real_modem_prototype";
  artifact.mode_descriptor.prototype = true;
  artifact.mode_descriptor.not_final_modem = true;
  artifact.mode_descriptor.downselect_valid = false;
  artifact.mode_descriptor.not_downselect_valid = true;
  artifact.mode_descriptor.performance_valid = false;
  artifact.mode_descriptor.performance_validity = "limited";
  artifact.mode_descriptor.downselect_validity = "invalid";
  artifact.mode_descriptor.codec_family = "synthetic";
  artifact.mode_descriptor.fec_family = "none";
  artifact.mode_descriptor.sync_family = "none";
  artifact.mode_descriptor.modem_family = "minimal_qpsk";
  artifact.mode_descriptor.prototype_limitations =
      "minimal QPSK-like prototype; no FEC; limited sync; not final performance";
  artifact.mode_descriptor.prototype_warning =
      "REAL MODEM PROTOTYPE WARNING: limited diagnostics only; performance_valid=limited";
  artifact.prototype_symbol_error_rate = 0.125;
  artifact.prototype_frame_status = "limited";
  artifact.prototype_sync_status = "coarse_sync_placeholder";
  artifact.prototype_baseband_sample_count = 1920;
  artifact.prototype_limitations =
      "diagnostic symbol/frame/sync values are prototype-only";
  artifact.warnings.push_back(artifact.mode_descriptor.prototype_warning);

  const auto json = f700f::metrics::to_json(artifact);
  assert(json.find("\"implementation_status\":\"real_modem_prototype\"") !=
         std::string::npos);
  assert(json.find("\"implementation_classification\":\"real_modem_prototype\"") !=
         std::string::npos);
  assert(json.find("\"performance_validity\":\"limited\"") !=
         std::string::npos);
  assert(json.find("\"downselect_validity\":\"invalid\"") !=
         std::string::npos);
  assert(json.find("\"sync_family\":\"none\"") != std::string::npos);
  assert(json.find("\"prototype_symbol_error_rate\":0.125") !=
         std::string::npos);
  assert(json.find("\"prototype_frame_status\":\"limited\"") !=
         std::string::npos);
  assert(json.find("\"prototype_sync_status\":\"coarse_sync_placeholder\"") !=
         std::string::npos);
  assert(json.find("\"prototype_baseband_sample_count\":1920") !=
         std::string::npos);

  auto parsed = f700f::metrics::from_json(json);
  assert(parsed.mode_descriptor.implementation_status ==
         "real_modem_prototype");
  assert(parsed.mode_descriptor.implementation_classification ==
         "real_modem_prototype");
  assert(!parsed.mode_descriptor.downselect_valid);
  assert(parsed.mode_descriptor.not_downselect_valid);
  assert(!parsed.mode_descriptor.performance_valid);
  assert(parsed.mode_descriptor.performance_validity == "limited");
  assert(parsed.mode_descriptor.downselect_validity == "invalid");
  assert(parsed.mode_descriptor.modem_family == "minimal_qpsk");
  assert(parsed.mode_descriptor.sync_family == "none");
  assert(parsed.mode_descriptor.prototype_warning.find("limited diagnostics") !=
         std::string::npos);
  assert(parsed.prototype_symbol_error_rate.has_value());
  assert(parsed.prototype_symbol_error_rate.value() == 0.125);
  assert(parsed.prototype_frame_status == "limited");
  assert(parsed.prototype_sync_status == "coarse_sync_placeholder");
  assert(parsed.prototype_baseband_sample_count == 1920);
  assert(parsed.prototype_limitations.find("prototype-only") !=
         std::string::npos);

  const auto extras = f700f::metrics::metric_column_names(artifact);
  const auto header = f700f::metrics::to_csv_header(extras);
  const auto row = f700f::metrics::to_csv_row(artifact, extras);
  parsed = f700f::metrics::from_csv_row(header, row);
  assert(parsed.mode_descriptor.implementation_status ==
         "real_modem_prototype");
  assert(parsed.mode_descriptor.performance_validity == "limited");
  assert(parsed.mode_descriptor.sync_family == "none");
  assert(parsed.prototype_symbol_error_rate.has_value());
  assert(parsed.prototype_symbol_error_rate.value() == 0.125);
  assert(parsed.prototype_frame_status == "limited");
  assert(parsed.prototype_sync_status == "coarse_sync_placeholder");
  assert(parsed.prototype_baseband_sample_count == 1920);
}

}  // namespace

int main() {
  test_empty_result_object();
  test_ssb_result_uses_na_for_bit_metrics();
  test_digital_result_records_bit_metrics();
  test_skipped_result();
  test_json_is_stable();
  test_csv_minimum_columns();
  test_unknown_optional_metrics_survive();
  test_surrogate_guardrails_survive_result_serialization();
  test_real_modem_prototype_diagnostics_survive_result_serialization();
  return 0;
}

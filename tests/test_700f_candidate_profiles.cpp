#include <f700f/candidate_profiles.hpp>
#include <f700f/metrics_artifacts.hpp>
#include <f700f/sweep_runner.hpp>
#include <f700f/types.hpp>

#include <cassert>
#include <cmath>
#include <set>
#include <string>
#include <vector>

namespace {

bool close_to(double lhs, double rhs) {
  return std::fabs(lhs - rhs) < 0.001;
}

void assert_common_candidate_shape(const f700f::ModeDescriptor &descriptor) {
  assert(!descriptor.mode_id.empty());
  assert(!descriptor.display_name.empty());
  assert(close_to(descriptor.rf_bandwidth_hz, 1900.0));
  assert(descriptor.audio_low_hz == 300.0);
  assert(descriptor.audio_high_hz > descriptor.audio_low_hz);
  assert(descriptor.audio_bandwidth_hz > 0.0);
  assert(descriptor.nominal_latency_s > 0.0);
  assert(descriptor.frame_duration_s > 0.0);
  assert(descriptor.raw_bitrate_bps > 0);
  assert(descriptor.voice_bitrate_bps > 0);
  assert(descriptor.carrier_count > 0);
  assert(!descriptor.codec_id.empty());
  assert(!descriptor.fec_id.empty());
  assert(!descriptor.modem_id.empty());
  assert(!descriptor.modulation_family.empty());
  assert(!descriptor.pilot_strategy.empty());
  assert(!descriptor.official_baseline);
  assert(!descriptor.emulator);
  assert(descriptor.implementation_status == "profile_only");
  assert(descriptor.capabilities.audio_input);
  assert(descriptor.capabilities.audio_output);
  assert(descriptor.capabilities.complex_input);
  assert(descriptor.capabilities.complex_output);
  assert(descriptor.capabilities.bit_payload);
}

void descriptors_are_obtainable_and_unique() {
  const std::vector<f700f::ModeDescriptor> descriptors = {
      f700f::freedv700f_a_balanced_descriptor(),
      f700f::freedv700f_b_robust_descriptor(),
      f700f::freedv700f_c_quality_descriptor(),
  };

  std::set<std::string> ids;
  for (const auto &descriptor : descriptors) {
    assert_common_candidate_shape(descriptor);
    ids.insert(descriptor.mode_id);
  }
  assert(ids.size() == 3);
  assert(ids.count("freedv700f_a_balanced") == 1);
  assert(ids.count("freedv700f_b_robust") == 1);
  assert(ids.count("freedv700f_c_quality") == 1);
}

void profile_targets_are_encoded_in_descriptors() {
  const auto &balanced = f700f::freedv700f_a_balanced_descriptor();
  const auto &robust = f700f::freedv700f_b_robust_descriptor();
  const auto &quality = f700f::freedv700f_c_quality_descriptor();

  assert(close_to(balanced.audio_high_hz, 3300.0));
  assert(close_to(balanced.audio_bandwidth_hz, 3000.0));
  assert(balanced.fec_id.find("medium") != std::string::npos);
  assert(balanced.implementation_status == "profile_only");

  assert(robust.audio_high_hz >= 3000.0);
  assert(robust.audio_high_hz <= 3300.0);
  assert(robust.audio_bandwidth_hz >= 2700.0);
  assert(robust.audio_bandwidth_hz <= 3000.0);
  assert(robust.fec_id.find("strong") != std::string::npos);
  assert(robust.nominal_latency_s >= balanced.nominal_latency_s);

  assert(quality.audio_high_hz >= 3600.0);
  assert(quality.audio_high_hz <= 4300.0);
  assert(quality.audio_bandwidth_hz >= 3300.0);
  assert(quality.audio_bandwidth_hz <= 4000.0);
  assert(quality.fec_id.find("light") != std::string::npos);
}

void registry_selects_candidate_profiles() {
  f700f::ModeRegistry registry;
  f700f::register_700f_candidate_profiles(registry);

  assert(registry.find("freedv700f_a_balanced") != nullptr);
  assert(registry.find("freedv700f_b_robust") != nullptr);
  assert(registry.find("freedv700f_c_quality") != nullptr);
  assert(registry.find("freedv700d_emulated") == nullptr);

  const auto *balanced = registry.find("freedv700f_a_balanced");
  assert(balanced != nullptr);
  assert(!balanced->official_baseline);
  assert(!balanced->emulator);
}

void encode_decode_report_profile_only_status() {
  f700f::ModeRegistry registry;
  f700f::register_700f_candidate_profiles(registry);
  auto mode = registry.create("freedv700f_a_balanced");
  assert(mode != nullptr);
  assert(mode->configure({.sample_rate_hz = mode->descriptor().sample_rate_hz}));

  const auto encoded = mode->encode({});
  assert(!encoded.ok);
  assert(encoded.error.find("freedv700f_a_balanced") != std::string::npos);
  assert(encoded.error.find("ISSUE-0018") != std::string::npos);
  assert(encoded.error.find("profile_only") != std::string::npos);

  const auto decoded = mode->decode({});
  assert(!decoded.ok);
  assert(decoded.error.find("freedv700f_a_balanced") != std::string::npos);
  assert(decoded.error.find("ISSUE-0018") != std::string::npos);
  assert(decoded.error.find("profile_only") != std::string::npos);
}

void metrics_snapshot_and_json_report_preserve_profile() {
  const auto &descriptor = f700f::freedv700f_c_quality_descriptor();
  const auto snapshot = f700f::metrics::make_mode_descriptor_snapshot(descriptor);

  assert(snapshot.mode_id == "freedv700f_c_quality");
  assert(close_to(snapshot.rf_bandwidth_hz, 1900.0));
  assert(close_to(snapshot.audio_high_hz, descriptor.audio_high_hz));
  assert(snapshot.codec_id == descriptor.codec_id);
  assert(snapshot.fec_id == descriptor.fec_id);
  assert(snapshot.modem_id == descriptor.modem_id);
  assert(snapshot.supports_bit_payload);

  auto artifact = f700f::metrics::make_empty_result_artifact();
  artifact.project_version = "v0.2.0";
  artifact.module_version = "v0.2.0";
  artifact.run_id = "candidate-profile-snapshot";
  artifact.mode_descriptor = snapshot;
  const auto json = f700f::metrics::to_json(artifact);
  assert(json.find("freedv700f_c_quality") != std::string::npos);
  assert(json.find("candidate-profile-snapshot") != std::string::npos);
}

void sweep_runner_reaches_profile_and_reports_profile_only_completion() {
  f700f::SweepRunner runner;
  runner.register_mode_factory(f700f::make_700f_candidate_profile_factory(
      "freedv700f_b_robust"));
  runner.register_channel_factory(f700f::make_identity_channel_factory());
  runner.register_metric(f700f::make_dummy_metric());

  f700f::SweepConfig config;
  config.run_id_prefix = "candidate-profile-sweep";
  config.input = f700f::GeneratedToneConfig{.sample_rate_hz = 8000,
                                            .sample_count = 8,
                                            .frequency_hz = 1000.0F,
                                            .amplitude = 0.25F};
  config.metric_ids = {"dummy.metric"};
  config.modes = {{.mode_id = "freedv700f_b_robust"}};
  config.channel_conditions = {{.condition_id = "identity",
                                .channel_chain = {{.channel_id = "identity"}}}};
  config.seeds = {18};

  const auto result = runner.run(config);
  assert(result.ok);
  assert(result.records.size() == 1);
  assert(result.records[0].status == f700f::SweepRunStatus::Completed);
  assert(result.records[0].mode_id == "freedv700f_b_robust");
  assert(result.records[0].error_summary.find("profile_only_completed") !=
         std::string::npos);

  const auto json = f700f::sweep_result_to_json(result);
  assert(json.find("freedv700f_b_robust") != std::string::npos);
  assert(json.find("completed") != std::string::npos);
  assert(json.find("profile_only_completed") != std::string::npos);
}

} // namespace

int main() {
  descriptors_are_obtainable_and_unique();
  profile_targets_are_encoded_in_descriptors();
  registry_selects_candidate_profiles();
  encode_decode_report_profile_only_status();
  metrics_snapshot_and_json_report_preserve_profile();
  sweep_runner_reaches_profile_and_reports_profile_only_completion();
  return 0;
}

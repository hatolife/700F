#include "f700f/channel_model.hpp"

#include <complex>
#include <cstdint>
#include <limits>
#include <cmath>

namespace {

double compute_power(const std::vector<float> &samples) {
  if (samples.empty()) {
    return 0.0;
  }
  double total = 0.0;
  for (const float sample : samples) {
    total += static_cast<double>(sample) * static_cast<double>(sample);
  }
  return total / static_cast<double>(samples.size());
}

double compute_power(const std::vector<f700f::Complex32> &samples) {
  if (samples.empty()) {
    return 0.0;
  }
  double total = 0.0;
  for (const auto &sample : samples) {
    total += static_cast<double>(sample.re) * static_cast<double>(sample.re) +
             static_cast<double>(sample.im) * static_cast<double>(sample.im);
  }
  return total / static_cast<double>(samples.size());
}

}  // namespace

namespace f700f {

AwgnChannel::AwgnChannel(double snr_db, Seed seed) : snr_db_(snr_db), seed_(seed), rng_(seed) {}

const char *AwgnChannel::name() const noexcept { return "awgn"; }

std::string_view AwgnChannel::last_error() const noexcept {
  return last_error_;
}

void AwgnChannel::clear_error() { last_error_.clear(); }

void AwgnChannel::reseed(Seed seed) {
  seed_ = seed;
  rng_.seed(seed);
}

void AwgnChannel::reset() {
  reseed(seed_);
  clear_error();
}

bool AwgnChannel::validate_snr() const {
  return std::isfinite(snr_db_) && snr_db_ > -300.0;
}

double AwgnChannel::compute_noise_std(double signal_power) const {
  if (signal_power <= 0.0) {
    return 0.0;
  }
  const double snr_linear = std::pow(10.0, snr_db_ / 10.0);
  return std::sqrt(signal_power / snr_linear);
}

bool AwgnChannel::apply_audio(AudioBlock &block) {
  const double signal_power = compute_power(block.mono);
  if (!validate_snr()) {
    last_error_ = "invalid SNR";
    return false;
  }
  const double noise_std = compute_noise_std(signal_power);
  if (noise_std == 0.0 || block.mono.empty()) {
    return true;
  }
  std::normal_distribution<float> distribution(0.0F, static_cast<float>(noise_std));
  for (auto &sample : block.mono) {
    sample += distribution(rng_);
  }
  return true;
}

bool AwgnChannel::apply_complex(ComplexBlock &block) {
  const double signal_power = compute_power(block.iq);
  if (!validate_snr()) {
    last_error_ = "invalid SNR";
    return false;
  }
  const double noise_std_total = compute_noise_std(signal_power);
  const double noise_std = noise_std_total / std::sqrt(2.0);
  if (noise_std == 0.0 || block.iq.empty()) {
    return true;
  }
  std::normal_distribution<float> distribution(0.0F, static_cast<float>(noise_std));
  for (auto &sample : block.iq) {
    sample.re += distribution(rng_);
    sample.im += distribution(rng_);
  }
  return true;
}

bool AwgnChannel::apply(AudioBlock &block) {
  clear_error();
  return apply_audio(block);
}

bool AwgnChannel::apply(ComplexBlock &block) {
  clear_error();
  return apply_complex(block);
}

FrequencyOffsetChannel::FrequencyOffsetChannel(double frequency_offset_hz)
    : frequency_offset_hz_(frequency_offset_hz) {}

const char *FrequencyOffsetChannel::name() const noexcept {
  return "frequency_offset";
}

void FrequencyOffsetChannel::clear_error() { last_error_.clear(); }

std::string_view FrequencyOffsetChannel::last_error() const noexcept {
  return last_error_;
}

void FrequencyOffsetChannel::set_frequency_offset(double frequency_offset_hz) noexcept {
  frequency_offset_hz_ = frequency_offset_hz;
}

double FrequencyOffsetChannel::wrap_phase(double phase) {
  constexpr double two_pi = 6.283185307179586476925286766559;
  if (phase >= 0.0) {
    return std::fmod(phase, two_pi);
  }
  return std::fmod(phase + std::ceil(std::abs(phase / two_pi)) * two_pi, two_pi);
}

bool FrequencyOffsetChannel::apply(AudioBlock &) {
  clear_error();
  return true;
}

bool FrequencyOffsetChannel::apply(ComplexBlock &block) {
  clear_error();
  if (block.sample_rate_hz == 0) {
    last_error_ = "invalid sample_rate_hz";
    return false;
  }
  if (block.iq.empty()) {
    return true;
  }
  if (frequency_offset_hz_ == 0.0) {
    return true;
  }
  const double delta_phase = 6.283185307179586476925286766559 *
                             (frequency_offset_hz_ / static_cast<double>(block.sample_rate_hz));
  double phase = wrap_phase(6.283185307179586476925286766559 *
                           (frequency_offset_hz_ * block.start_time_s));
  for (auto &sample : block.iq) {
    const float i = sample.re;
    const float q = sample.im;
    const float cos_phase = static_cast<float>(std::cos(phase));
    const float sin_phase = static_cast<float>(std::sin(phase));
    sample.re = i * cos_phase - q * sin_phase;
    sample.im = i * sin_phase + q * cos_phase;
    phase = wrap_phase(phase + delta_phase);
  }
  return true;
}

SimpleGainFadingChannel::SimpleGainFadingChannel(double min_gain_db, double max_gain_db, Seed seed)
    : min_gain_db_(min_gain_db), max_gain_db_(max_gain_db), seed_(seed), rng_(seed) {}

const char *SimpleGainFadingChannel::name() const noexcept {
  return "simple_gain_fading";
}

std::string_view SimpleGainFadingChannel::last_error() const noexcept {
  return last_error_;
}

void SimpleGainFadingChannel::clear_error() { last_error_.clear(); }

void SimpleGainFadingChannel::reseed(Seed seed) {
  seed_ = seed;
  rng_.seed(seed);
}

void SimpleGainFadingChannel::reset() {
  reseed(seed_);
  clear_error();
}

bool SimpleGainFadingChannel::validate_limits() const {
  return std::isfinite(min_gain_db_) && std::isfinite(max_gain_db_) &&
         min_gain_db_ <= max_gain_db_;
}

double SimpleGainFadingChannel::draw_gain_db() const {
  std::uniform_real_distribution<double> distribution(min_gain_db_, max_gain_db_);
  return distribution(rng_);
}

float SimpleGainFadingChannel::sample_gain() const {
  const double gain_db = draw_gain_db();
  return static_cast<float>(std::pow(10.0, gain_db / 20.0));
}

bool SimpleGainFadingChannel::apply(AudioBlock &block) {
  clear_error();
  if (!validate_limits()) {
    last_error_ = "invalid gain range";
    return false;
  }
  const float gain = sample_gain();
  if (block.mono.empty()) {
    return true;
  }
  for (auto &sample : block.mono) {
    sample *= gain;
  }
  return true;
}

bool SimpleGainFadingChannel::apply(ComplexBlock &block) {
  clear_error();
  if (!validate_limits()) {
    last_error_ = "invalid gain range";
    return false;
  }
  const float gain = sample_gain();
  if (block.iq.empty()) {
    return true;
  }
  for (auto &sample : block.iq) {
    sample.re *= gain;
    sample.im *= gain;
  }
  return true;
}

ChannelChain::ChannelChain(std::vector<std::unique_ptr<ChannelEffect>> effects) {
  effects_ = std::move(effects);
}

void ChannelChain::add(std::unique_ptr<ChannelEffect> effect) {
  effects_.push_back(std::move(effect));
}

void ChannelChain::clear() {
  effects_.clear();
  clear_error();
}

bool ChannelChain::empty() const noexcept {
  return effects_.empty();
}

std::size_t ChannelChain::size() const noexcept {
  return effects_.size();
}

void ChannelChain::clear_error() { last_error_.clear(); }

std::string_view ChannelChain::last_error() const noexcept {
  return last_error_;
}

bool ChannelChain::apply(AudioBlock &block) {
  clear_error();
  for (const auto &effect : effects_) {
    if (!effect->apply(block)) {
      last_error_ = effect->last_error();
      return false;
    }
  }
  return true;
}

bool ChannelChain::apply(ComplexBlock &block) {
  clear_error();
  for (const auto &effect : effects_) {
    if (!effect->apply(block)) {
      last_error_ = effect->last_error();
      return false;
    }
  }
  return true;
}

}  // namespace f700f

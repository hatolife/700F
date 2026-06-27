#include <f700f/channel_model.hpp>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

namespace {

f700f::AudioBlock make_audio_block(std::uint32_t sample_rate) {
  return {sample_rate, {0.0F, 0.1F, -0.2F, 0.3F, -0.4F, 0.5F}, 0.0};
}

f700f::ComplexBlock make_complex_block(std::uint32_t sample_rate) {
  return {sample_rate, {{0.5F, 0.1F}, {-0.2F, 0.4F}, {0.3F, -0.5F}, {-0.6F, 0.7F}}, 150.0, 0.0};
}

double mean_squared_error(const f700f::AudioBlock &a, const f700f::AudioBlock &b) {
  assert(a.mono.size() == b.mono.size());
  if (a.mono.empty()) {
    return 0.0;
  }
  double total = 0.0;
  for (std::size_t i = 0; i < a.mono.size(); ++i) {
    const double diff = static_cast<double>(a.mono[i] - b.mono[i]);
    total += diff * diff;
  }
  return total / static_cast<double>(a.mono.size());
}

double mean_squared_error(const f700f::ComplexBlock &a, const f700f::ComplexBlock &b) {
  assert(a.iq.size() == b.iq.size());
  if (a.iq.empty()) {
    return 0.0;
  }
  double total = 0.0;
  for (std::size_t i = 0; i < a.iq.size(); ++i) {
    const double dr = static_cast<double>(a.iq[i].re - b.iq[i].re);
    const double di = static_cast<double>(a.iq[i].im - b.iq[i].im);
    total += dr * dr + di * di;
  }
  return total / (2.0 * static_cast<double>(a.iq.size()));
}

void awgn_same_seed_deterministic_test() {
  const auto clean = make_complex_block(8000);
  auto block1 = clean;
  auto block2 = clean;

  f700f::AwgnChannel awgn1(12.0, 7);
  f700f::AwgnChannel awgn2(12.0, 7);
  assert(awgn1.apply(block1));
  assert(awgn2.apply(block2));
  assert(block1.iq.size() == block2.iq.size());
  assert(mean_squared_error(block1, block2) == 0.0);
}

void awgn_different_seed_produces_different_output_test() {
  auto input = make_complex_block(8000);
  auto block1 = input;
  auto block2 = input;

  f700f::AwgnChannel awgn1(15.0, 1);
  f700f::AwgnChannel awgn2(15.0, 2);
  assert(awgn1.apply(block1));
  assert(awgn2.apply(block2));

  assert(mean_squared_error(block1, block2) > 0.0);
}

void awgn_snr_variance_trend_test() {
  auto clean = make_complex_block(8000);
  auto low_snr_block = clean;
  auto high_snr_block = clean;

  f700f::AwgnChannel low_snr(0.0, 123);
  f700f::AwgnChannel high_snr(40.0, 123);

  const auto base = clean;
  assert(low_snr.apply(low_snr_block));
  high_snr.reseed(123);
  assert(high_snr.apply(high_snr_block));

  const double low_error = mean_squared_error(low_snr_block, base);
  const double high_error = mean_squared_error(high_snr_block, base);
  assert(high_error < low_error);
}

void frequency_offset_zero_hz_is_noop_test() {
  auto input = make_complex_block(8000);
  auto untouched = input;
  f700f::FrequencyOffsetChannel offset(0.0);
  assert(offset.apply(input));
  assert(mean_squared_error(input, untouched) < 1e-12);
}

void gain_fading_reproducible_test() {
  auto audio = make_audio_block(8000);
  auto block1 = audio;
  auto block2 = audio;

  f700f::SimpleGainFadingChannel gain1(-6.0, 3.0, 55);
  f700f::SimpleGainFadingChannel gain2(-6.0, 3.0, 55);
  assert(gain1.apply(block1));
  assert(gain2.apply(block2));
  assert(block1.mono == block2.mono);
}

void channel_chain_order_preserved_test() {
  auto input = make_complex_block(8000);

  f700f::ChannelChain chain_a;
  chain_a.add(std::make_unique<f700f::AwgnChannel>(20.0, 99));
  chain_a.add(std::make_unique<f700f::FrequencyOffsetChannel>(50.0));

  f700f::ChannelChain chain_b;
  chain_b.add(std::make_unique<f700f::FrequencyOffsetChannel>(50.0));
  chain_b.add(std::make_unique<f700f::AwgnChannel>(20.0, 99));

  auto block_a = input;
  auto block_b = input;

  assert(chain_a.apply(block_a));
  assert(chain_b.apply(block_b));
  assert(mean_squared_error(block_a, block_b) > 0.0);
}

void zero_length_block_does_not_crash_test() {
  f700f::AudioBlock empty_audio{8000, {}, 0.0};
  f700f::ComplexBlock empty_complex{8000, {}, 0.0, 0.0};

  f700f::AwgnChannel awgn(10.0, 5);
  f700f::FrequencyOffsetChannel offset(100.0);
  f700f::SimpleGainFadingChannel gain(-4.0, 4.0, 6);
  f700f::ChannelChain chain;
  chain.add(std::make_unique<f700f::AwgnChannel>(10.0, 5));
  chain.add(std::make_unique<f700f::FrequencyOffsetChannel>(100.0));
  chain.add(std::make_unique<f700f::SimpleGainFadingChannel>(-4.0, 4.0, 6));

  assert(awgn.apply(empty_audio));
  assert(offset.apply(empty_complex));
  assert(gain.apply(empty_complex));
  assert(chain.apply(empty_audio));
  assert(chain.apply(empty_complex));
  assert(empty_audio.mono.empty());
  assert(empty_complex.iq.empty());
}

void invalid_sample_rate_rejected_test() {
  auto invalid = make_complex_block(0);
  f700f::FrequencyOffsetChannel offset(1000.0);
  assert(!offset.apply(invalid));
  assert(!offset.last_error().empty());
}

}  // namespace

int main() {
  awgn_same_seed_deterministic_test();
  awgn_different_seed_produces_different_output_test();
  awgn_snr_variance_trend_test();
  frequency_offset_zero_hz_is_noop_test();
  gain_fading_reproducible_test();
  channel_chain_order_preserved_test();
  zero_length_block_does_not_crash_test();
  invalid_sample_rate_rejected_test();
  return 0;
}

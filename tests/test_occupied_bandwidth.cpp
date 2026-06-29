#include <f700f/occupied_bandwidth.hpp>

#include <cassert>
#include <cmath>
#include <complex>
#include <stdexcept>
#include <vector>

namespace {

constexpr double kPi = 3.14159265358979323846;

std::vector<std::complex<float>> tone(double frequency_hz,
                                      double sample_rate_hz,
                                      std::size_t sample_count) {
  std::vector<std::complex<float>> samples;
  samples.reserve(sample_count);
  for (std::size_t n = 0; n < sample_count; ++n) {
    const double phase = 2.0 * kPi * frequency_hz *
                         static_cast<double>(n) / sample_rate_hz;
    samples.push_back({static_cast<float>(std::cos(phase)),
                       static_cast<float>(std::sin(phase))});
  }
  return samples;
}

std::vector<std::complex<float>> two_tone(double first_hz,
                                          double second_hz,
                                          double sample_rate_hz,
                                          std::size_t sample_count) {
  auto samples = tone(first_hz, sample_rate_hz, sample_count);
  const auto other = tone(second_hz, sample_rate_hz, sample_count);
  for (std::size_t i = 0; i < samples.size(); ++i) {
    samples[i] = (samples[i] + other[i]) * 0.5f;
  }
  return samples;
}

void empty_signal_is_safe() {
  const auto measurement =
      f700f::metrics::measure_occupied_bandwidth({}, 8000.0);
  assert(measurement.occupied_bandwidth_hz == 0.0);
  assert(measurement.total_energy == 0.0);
  assert(measurement.status == "empty_signal");
}

void zero_signal_is_safe() {
  const std::vector<std::complex<float>> samples(256, {0.0f, 0.0f});
  const auto measurement =
      f700f::metrics::measure_occupied_bandwidth(samples, 8000.0);
  assert(measurement.occupied_bandwidth_hz == 0.0);
  assert(measurement.low_frequency_hz == 0.0);
  assert(measurement.high_frequency_hz == 0.0);
  assert(measurement.status == "zero_energy");
}

void invalid_sample_rate_is_rejected() {
  bool rejected = false;
  try {
    (void)f700f::metrics::measure_occupied_bandwidth(tone(1000.0, 8000.0, 128),
                                                     0.0);
  } catch (const std::invalid_argument &) {
    rejected = true;
  }
  assert(rejected);
}

void single_tone_estimate_is_stable() {
  const auto measurement =
      f700f::metrics::measure_occupied_bandwidth(tone(1000.0, 8000.0, 1024),
                                                 8000.0);
  assert(measurement.status == "measured");
  assert(measurement.occupied_bandwidth_hz > 0.0);
  assert(measurement.occupied_bandwidth_hz < 20.0);
  assert(std::abs(measurement.center_frequency_hz - 1000.0) < 1.0);
}

void two_tone_estimate_is_wider_than_single_tone() {
  const auto single =
      f700f::metrics::measure_occupied_bandwidth(tone(1000.0, 8000.0, 1024),
                                                 8000.0);
  const auto wider = f700f::metrics::measure_occupied_bandwidth(
      two_tone(-500.0, 500.0, 8000.0, 1024), 8000.0);
  assert(wider.status == "measured");
  assert(wider.occupied_bandwidth_hz > single.occupied_bandwidth_hz);
  assert(wider.occupied_bandwidth_hz > 990.0);
  assert(wider.occupied_bandwidth_hz < 1020.0);
}

}  // namespace

int main() {
  empty_signal_is_safe();
  zero_signal_is_safe();
  invalid_sample_rate_is_rejected();
  single_tone_estimate_is_stable();
  two_tone_estimate_is_wider_than_single_tone();
  return 0;
}

#include <f700f/occupied_bandwidth.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace f700f::metrics {
namespace {

constexpr double kPi = 3.14159265358979323846;

struct SpectralBin {
  double frequency_hz = 0.0;
  double energy = 0.0;
};

double bin_frequency(std::size_t index, std::size_t size, double sample_rate_hz) {
  const double bin_width = sample_rate_hz / static_cast<double>(size);
  if (index <= size / 2) {
    return static_cast<double>(index) * bin_width;
  }
  return (static_cast<double>(index) - static_cast<double>(size)) * bin_width;
}

std::vector<SpectralBin> dft_energy_bins(
    const std::vector<std::complex<float>> &samples,
    double sample_rate_hz) {
  std::vector<SpectralBin> bins;
  bins.reserve(samples.size());
  const auto size = samples.size();
  for (std::size_t k = 0; k < size; ++k) {
    std::complex<double> acc{0.0, 0.0};
    for (std::size_t n = 0; n < size; ++n) {
      const double phase = -2.0 * kPi * static_cast<double>(k) *
                           static_cast<double>(n) / static_cast<double>(size);
      const std::complex<double> twiddle{std::cos(phase), std::sin(phase)};
      acc += std::complex<double>{samples[n].real(), samples[n].imag()} * twiddle;
    }
    bins.push_back({bin_frequency(k, size, sample_rate_hz), std::norm(acc)});
  }
  std::sort(bins.begin(), bins.end(), [](const auto &lhs, const auto &rhs) {
    return lhs.frequency_hz < rhs.frequency_hz;
  });
  return bins;
}

}  // namespace

OccupiedBandwidthMeasurement measure_occupied_bandwidth(
    const std::vector<std::complex<float>> &samples,
    double sample_rate_hz,
    double occupied_ratio) {
  if (!std::isfinite(sample_rate_hz) || sample_rate_hz <= 0.0) {
    throw std::invalid_argument("occupied bandwidth sample rate must be positive");
  }
  if (!std::isfinite(occupied_ratio) || occupied_ratio <= 0.0 ||
      occupied_ratio > 1.0) {
    throw std::invalid_argument("occupied bandwidth ratio must be in (0, 1]");
  }

  OccupiedBandwidthMeasurement measurement;
  measurement.sample_rate_hz = sample_rate_hz;
  measurement.occupied_ratio = occupied_ratio;
  measurement.sample_count = samples.size();

  if (samples.empty()) {
    measurement.status = "empty_signal";
    return measurement;
  }

  auto bins = dft_energy_bins(samples, sample_rate_hz);
  measurement.total_energy = std::accumulate(
      bins.begin(), bins.end(), 0.0,
      [](double total, const SpectralBin &bin) { return total + bin.energy; });
  if (measurement.total_energy <= std::numeric_limits<double>::epsilon()) {
    measurement.status = "zero_energy";
    return measurement;
  }

  const double tail_energy =
      measurement.total_energy * std::max(0.0, 1.0 - occupied_ratio) / 2.0;
  const double high_tail_limit = measurement.total_energy - tail_energy;
  double cumulative = 0.0;
  std::size_t low_index = 0;
  while (low_index + 1 < bins.size() &&
         cumulative + bins[low_index].energy <= tail_energy) {
    cumulative += bins[low_index].energy;
    ++low_index;
  }

  cumulative = 0.0;
  std::size_t high_index = bins.size() - 1;
  for (std::size_t i = 0; i < bins.size(); ++i) {
    cumulative += bins[i].energy;
    if (cumulative >= high_tail_limit) {
      high_index = i;
      break;
    }
  }

  if (high_index < low_index) {
    high_index = low_index;
  }

  const double bin_width = sample_rate_hz / static_cast<double>(samples.size());
  measurement.low_frequency_hz = bins[low_index].frequency_hz - bin_width / 2.0;
  measurement.high_frequency_hz = bins[high_index].frequency_hz + bin_width / 2.0;
  measurement.occupied_bandwidth_hz =
      std::max(0.0, measurement.high_frequency_hz - measurement.low_frequency_hz);
  measurement.center_frequency_hz =
      (measurement.low_frequency_hz + measurement.high_frequency_hz) / 2.0;
  measurement.status = "measured";
  return measurement;
}

}  // namespace f700f::metrics

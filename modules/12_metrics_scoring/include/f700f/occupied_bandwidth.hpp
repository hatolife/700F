#pragma once

#include <complex>
#include <cstddef>
#include <string>
#include <vector>

namespace f700f::metrics {

struct OccupiedBandwidthMeasurement {
  double sample_rate_hz = 0.0;
  double occupied_ratio = 0.99;
  double occupied_bandwidth_hz = 0.0;
  double low_frequency_hz = 0.0;
  double high_frequency_hz = 0.0;
  double center_frequency_hz = 0.0;
  double total_energy = 0.0;
  std::size_t sample_count = 0;
  std::string status = "not_measured";
};

OccupiedBandwidthMeasurement measure_occupied_bandwidth(
    const std::vector<std::complex<float>> &samples,
    double sample_rate_hz,
    double occupied_ratio = 0.99);

}  // namespace f700f::metrics

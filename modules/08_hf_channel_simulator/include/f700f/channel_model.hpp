#pragma once

#include <f700f/types.hpp>

#include <cmath>
#include <memory>
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace f700f {

class ChannelEffect {
public:
  virtual ~ChannelEffect() = default;

  virtual const char *name() const noexcept = 0;
  virtual bool apply(AudioBlock &block) = 0;
  virtual bool apply(ComplexBlock &block) = 0;
  virtual std::string_view last_error() const noexcept = 0;
};

class AwgnChannel final : public ChannelEffect {
public:
  AwgnChannel(double snr_db, Seed seed);

  const char *name() const noexcept override;
  bool apply(AudioBlock &block) override;
  bool apply(ComplexBlock &block) override;
  std::string_view last_error() const noexcept override;
  void reseed(Seed seed);
  void reset();

private:
  bool apply_audio(AudioBlock &block);
  bool apply_complex(ComplexBlock &block);

  bool validate_snr() const;
  void clear_error();
  double compute_noise_std(double signal_power) const;

  double snr_db_ = 30.0;
  Seed seed_ = 0;
  mutable std::string last_error_;
  std::mt19937_64 rng_;
};

class FrequencyOffsetChannel final : public ChannelEffect {
public:
  explicit FrequencyOffsetChannel(double frequency_offset_hz);

  const char *name() const noexcept override;
  bool apply(AudioBlock &block) override;
  bool apply(ComplexBlock &block) override;
  std::string_view last_error() const noexcept override;
  void set_frequency_offset(double frequency_offset_hz) noexcept;

private:
  void clear_error();
  static double wrap_phase(double phase);

  double frequency_offset_hz_ = 0.0;
  mutable std::string last_error_;
};

class SimpleGainFadingChannel final : public ChannelEffect {
public:
  SimpleGainFadingChannel(double min_gain_db, double max_gain_db, Seed seed);

  const char *name() const noexcept override;
  bool apply(AudioBlock &block) override;
  bool apply(ComplexBlock &block) override;
  std::string_view last_error() const noexcept override;
  void reseed(Seed seed);
  void reset();

private:
  void clear_error();
  bool validate_limits() const;
  double draw_gain_db() const;
  float sample_gain() const;

  double min_gain_db_ = -6.0;
  double max_gain_db_ = 6.0;
  Seed seed_ = 0;
  mutable std::string last_error_;
  mutable std::mt19937_64 rng_;
};

class ChannelChain final {
public:
  explicit ChannelChain(std::vector<std::unique_ptr<ChannelEffect>> effects = {});

  void add(std::unique_ptr<ChannelEffect> effect);
  bool apply(AudioBlock &block);
  bool apply(ComplexBlock &block);
  void clear();
  bool empty() const noexcept;
  std::size_t size() const noexcept;
  std::string_view last_error() const noexcept;

private:
  void clear_error();
  std::vector<std::unique_ptr<ChannelEffect>> effects_;
  std::string last_error_;
};

}  // namespace f700f

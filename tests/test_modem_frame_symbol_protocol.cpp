#include <f700f/types.hpp>

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace {

void default_construction_is_safe() {
  f700f::ModemInputFrame input;
  f700f::SymbolBlock symbols;
  f700f::BasebandFrame baseband;
  f700f::ModemOutputFrame output;

  assert(input.frame_index == 0);
  assert(input.payload_bytes.empty());
  assert(input.descriptor.sample_rate.hz == 0);
  assert(symbols.symbols.empty());
  assert(baseband.samples.empty());
  assert(output.decoded_payload_bytes.empty());
  assert(output.status.status == "limited");
  assert(output.status.prototype);
  assert(output.status.not_final_modem);
  assert(!output.status.downselect_valid);
}

void descriptor_propagates_report_visible_metadata() {
  f700f::ModemFrameDescriptor descriptor;
  descriptor.mode_id = "freedv700f_a_balanced";
  descriptor.frame_index = 7;
  descriptor.sample_rate = f700f::SampleRate{.hz = 8000};
  descriptor.symbol_rate = f700f::SymbolRate{.baud = 1200.0};
  descriptor.modem_family = "minimal_qpsk_baseband";
  descriptor.fec.family = "placeholder";
  descriptor.sync.family = "symbol_clock_placeholder";
  descriptor.codec.family = "synthetic";
  descriptor.carrier_layout.family = "single_carrier_placeholder";
  descriptor.limitations = {"ISSUE-0043 protocol only; no waveform behavior"};

  f700f::ModemInputFrame input;
  input.descriptor = descriptor;
  input.payload_bytes = {0x70, 0x0F};
  input.seed = 43;

  f700f::SymbolBlock symbols;
  symbols.descriptor = input.descriptor;
  symbols.bits_per_symbol = 2;
  symbols.symbols = {{1.0F, 0.0F}, {0.0F, -1.0F}};
  symbols.limitations = input.descriptor.limitations;

  f700f::BasebandFrame baseband;
  baseband.descriptor = input.descriptor;
  baseband.sample_rate = input.descriptor.sample_rate;
  baseband.symbol_rate = input.descriptor.symbol_rate;
  baseband.samples = symbols.symbols;
  baseband.occupied_bandwidth_hz = 1900.0;

  f700f::ModemOutputFrame output;
  output.descriptor = input.descriptor;
  output.decoded_payload_bytes = input.payload_bytes;
  output.status.status = "limited";
  output.status.reason = "boundary contract exercised";
  output.status.limitations = input.descriptor.limitations;
  output.symbol_error_count = 0;
  output.symbol_count = symbols.symbols.size();
  output.prototype_metrics.emplace_back("prototype_symbol_count=2");

  assert(output.descriptor.mode_id == "freedv700f_a_balanced");
  assert(output.descriptor.modem_family == "minimal_qpsk_baseband");
  assert(output.descriptor.fec.family == "placeholder");
  assert(output.descriptor.sync.family == "symbol_clock_placeholder");
  assert(output.descriptor.codec.family == "synthetic");
  assert(output.descriptor.carrier_layout.family == "single_carrier_placeholder");
  assert(output.decoded_payload_bytes == std::vector<std::uint8_t>({0x70, 0x0F}));
  assert(output.symbol_count == 2);
  assert(output.prototype_metrics.front() == "prototype_symbol_count=2");
  assert(output.status.implementation_status == "real_modem_prototype");
  assert(output.status.performance_valid == "limited");
  assert(!output.status.limitations.empty());
}

void invalid_rates_are_detected() {
  assert(!f700f::SampleRate{.hz = 0}.valid());
  assert(f700f::SampleRate{.hz = 8000}.valid());
  assert(!f700f::SymbolRate{.baud = 0.0}.valid());
  assert(!f700f::SymbolRate{.baud = -1.0}.valid());
  assert(f700f::SymbolRate{.baud = 1200.0}.valid());

  f700f::ModemFrameDescriptor descriptor;
  descriptor.sample_rate = f700f::SampleRate{.hz = 8000};
  descriptor.symbol_rate = f700f::SymbolRate{.baud = 1200.0};
  assert(descriptor.valid_rates());
  descriptor.symbol_rate = f700f::SymbolRate{.baud = 0.0};
  assert(!descriptor.valid_rates());
}

void zero_length_frames_remain_valid_containers() {
  f700f::ModemFrameDescriptor descriptor;
  descriptor.mode_id = "freedv700f_a_balanced";
  descriptor.sample_rate = f700f::SampleRate{.hz = 8000};
  descriptor.symbol_rate = f700f::SymbolRate{.baud = 1200.0};

  f700f::ModemInputFrame input;
  input.descriptor = descriptor;
  f700f::SymbolBlock symbols;
  symbols.descriptor = descriptor;
  f700f::BasebandFrame baseband;
  baseband.descriptor = descriptor;
  f700f::ModemOutputFrame output;
  output.descriptor = descriptor;

  assert(input.payload_bytes.empty());
  assert(symbols.symbols.empty());
  assert(baseband.samples.empty());
  assert(output.decoded_payload_bytes.empty());
  assert(input.descriptor.valid_rates());
  assert(symbols.descriptor.valid_rates());
  assert(baseband.descriptor.valid_rates());
  assert(output.descriptor.valid_rates());
}

} // namespace

int main() {
  default_construction_is_safe();
  descriptor_propagates_report_visible_metadata();
  invalid_rates_are_detected();
  zero_length_frames_remain_valid_containers();
  return 0;
}

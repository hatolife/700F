#pragma once

#include <f700f/types.hpp>

#include <memory>

namespace f700f {

const ModeDescriptor &freedv700f_a_balanced_descriptor() noexcept;
const ModeDescriptor &freedv700f_b_robust_descriptor() noexcept;
const ModeDescriptor &freedv700f_c_quality_descriptor() noexcept;

ModemInputFrame make_700f_a_minimal_qpsk_input_frame(
    const AudioBlock &audio, std::uint64_t frame_index, Seed seed);
SymbolBlock encode_700f_a_minimal_qpsk_symbols(
    const ModemInputFrame &input);
BasebandFrame modulate_700f_a_minimal_qpsk_baseband(
    const SymbolBlock &symbols);
ModemOutputFrame demodulate_700f_a_minimal_qpsk_baseband(
    const BasebandFrame &baseband, std::uint64_t expected_audio_samples);

std::shared_ptr<IModeFactory> make_700f_candidate_profile_factory(
    ModeId mode_id);
void register_700f_candidate_profiles(ModeRegistry &registry);

} // namespace f700f

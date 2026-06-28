#pragma once

#include <f700f/types.hpp>

namespace f700f {

const ModeDescriptor &freedv700d_emulated_descriptor() noexcept;
const ModeDescriptor &freedv700e_emulated_descriptor() noexcept;
const ModeDescriptor &ssb_standard_3k_descriptor() noexcept;
const ModeDescriptor &ssb_narrow_1k9_descriptor() noexcept;
const char *freedv_emulator_model_name() noexcept;
const char *freedv_emulator_model_version() noexcept;
const char *freedv_emulator_limitations() noexcept;

void register_freedv_emulator_modes(ModeRegistry &registry);
void register_ssb_reference_modes(ModeRegistry &registry);
std::shared_ptr<IModeFactory> make_freedv700d_emulated_mode_factory();
std::shared_ptr<IModeFactory> make_freedv700e_emulated_mode_factory();
std::shared_ptr<IModeFactory> make_ssb_standard_3k_mode_factory();
std::shared_ptr<IModeFactory> make_ssb_narrow_1k9_mode_factory();

} // namespace f700f

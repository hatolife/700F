#pragma once

#include <f700f/types.hpp>

namespace f700f {

const ModeDescriptor &freedv700d_emulated_descriptor() noexcept;
const ModeDescriptor &freedv700e_emulated_descriptor() noexcept;

void register_freedv_emulator_modes(ModeRegistry &registry);

} // namespace f700f

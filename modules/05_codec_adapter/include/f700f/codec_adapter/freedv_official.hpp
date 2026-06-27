#pragma once

#include <f700f/types.hpp>

namespace f700f {

const ModeDescriptor &freedv700d_official_descriptor() noexcept;
const ModeDescriptor &freedv700e_official_descriptor() noexcept;

bool freedv_official_codec2_available() noexcept;
int freedv_official_codec2_mode_id(const ModeId &mode_id) noexcept;

void register_freedv_official_modes(ModeRegistry &registry);

} // namespace f700f

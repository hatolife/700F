#pragma once

#include <f700f/types.hpp>

#include <memory>

namespace f700f {

const ModeDescriptor &freedv700d_official_descriptor() noexcept;
const ModeDescriptor &freedv700e_official_descriptor() noexcept;

bool freedv_official_codec2_available() noexcept;
int freedv_official_codec2_mode_id(const ModeId &mode_id) noexcept;

void register_freedv_official_modes(ModeRegistry &registry);
std::shared_ptr<IModeFactory> make_freedv700d_official_mode_factory();
std::shared_ptr<IModeFactory> make_freedv700e_official_mode_factory();

} // namespace f700f

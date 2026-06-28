#pragma once

#include <f700f/types.hpp>

#include <memory>

namespace f700f {

const ModeDescriptor &freedv700f_a_balanced_descriptor() noexcept;
const ModeDescriptor &freedv700f_b_robust_descriptor() noexcept;
const ModeDescriptor &freedv700f_c_quality_descriptor() noexcept;

std::shared_ptr<IModeFactory> make_700f_candidate_profile_factory(
    ModeId mode_id);
void register_700f_candidate_profiles(ModeRegistry &registry);

} // namespace f700f

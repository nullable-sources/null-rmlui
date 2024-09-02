#include "drop-shadow.h"

namespace ntl::rml::directx9 {
    void c_drop_shadow_shader::set_constants(const constants_t& constants) {
        pixel_shader->set_constant(0, constants.uv_limits.linear_corners.data(), 4);
        pixel_shader->set_constant(1, constants.color.channels.data());
    }
}
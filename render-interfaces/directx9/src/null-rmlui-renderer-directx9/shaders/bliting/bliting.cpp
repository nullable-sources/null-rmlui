#include "bliting.h"

namespace ntl::rml::directx9 {
    void c_bliting_shader::set_constants(const constants_t& constants) {
        pixel_shader->set_constant(0, constants.uv_limits.linear_corners.data());
    }
}
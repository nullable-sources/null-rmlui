#include "bliting.h"

namespace ntl::rml::directx11 {
    void c_bliting_shader::set_constants(const constants_t& constants) {
        bliting_shader_object.set_constant(constants);
    }
}
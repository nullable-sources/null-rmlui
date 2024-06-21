#include "color-matrix.h"

namespace null::rml::directx11 {
    void c_color_matrix_shader::set_constants(const constants_t& constants) {
        color_matrix_shader_object.set_constant(constants);
    }
}
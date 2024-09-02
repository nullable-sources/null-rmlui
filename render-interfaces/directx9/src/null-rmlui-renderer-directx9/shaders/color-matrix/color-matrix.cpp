#include "color-matrix.h"

namespace ntl::rml::directx9 {
    void c_color_matrix_shader::set_constants(const constants_t& constants) {
        pixel_shader->set_constant(0, constants.color_matrix.linear_array.data(), 4);
    }
}
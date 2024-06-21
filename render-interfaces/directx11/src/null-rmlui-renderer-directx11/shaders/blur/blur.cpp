#include "blur.h"

namespace null::rml::directx11 {
    void c_blur_shader::set_constants(const constants_t& constants) {
        blur_vertex_shader_object.set_constant({ constants.texel_offset });

        c_blur_pixel_shader_object::constant_buffer_t pixel_constant_buffer{ };
        pixel_constant_buffer.uv_limits = constants.uv_limits;
        for(auto [i, weight] : constants.weights | std::views::enumerate) {
            pixel_constant_buffer.weights[i].x = weight;
        }
        blur_pixel_shader_object.set_constant(pixel_constant_buffer);
    }
}
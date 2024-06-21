#include "blur.h"

namespace null::rml::directx9 {
    void c_blur_shader::set_constants(const constants_t& constants) {
        vertex_shader->set_constant(1, constants.texel_offset.coordinates.data());

        pixel_shader->set_constant(0, constants.uv_limits.min.coordinates.data());
        pixel_shader->set_constant(1, constants.uv_limits.max.coordinates.data());

        std::vector<vec4_t<float>> align_weights{ };
        for(float weight : constants.weights) {
            align_weights.push_back(vec4_t(weight));
        }
        pixel_shader->set_constant(2, (float*)align_weights.data(), _num_weights);
    }
}
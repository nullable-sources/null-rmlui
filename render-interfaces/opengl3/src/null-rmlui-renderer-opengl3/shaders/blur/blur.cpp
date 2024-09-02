#include "blur.h"

namespace ntl::rml::opengl3 {
    void c_blur_shader::create() {
        if(!empty()) return;
        c_shader::create();

        texel_offset.get_location(program.get(), "texel_offset");
        uv_min.get_location(program.get(), "uv_min");
        uv_max.get_location(program.get(), "uv_max");
        weights.get_location(program.get(), "weights");
    }

    void c_blur_shader::use() {
        if(empty()) return;
        c_shader::use();

        texel_offset.set();
        uv_min.set();
        uv_max.set();
        weights.set();
    }

    void c_blur_shader::set_constants(const constants_t& constants) {
        texel_offset.value() = constants.texel_offset;

        uv_min.value().x = constants.uv_limits.min.x;
        uv_min.value().y = 1.f - constants.uv_limits.max.y;

        uv_max.value().x = constants.uv_limits.max.x;
        uv_max.value().y = 1.f - constants.uv_limits.min.y;

        for(auto [weight_dst, weight_src] : std::views::zip(weights.value(), constants.weights)) {
            weight_dst = weight_src;
        }
    }
}
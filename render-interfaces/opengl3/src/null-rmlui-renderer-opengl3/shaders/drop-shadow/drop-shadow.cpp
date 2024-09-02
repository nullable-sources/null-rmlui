#include "drop-shadow.h"

namespace ntl::rml::opengl3 {
    void c_drop_shadow_shader::create() {
        if(!empty()) return;
        c_shader::create();

        color.get_location(program.get(), "color");
        uv_min.get_location(program.get(), "uv_min");
        uv_max.get_location(program.get(), "uv_max");
    }

    void c_drop_shadow_shader::use() {
        if(empty()) return;
        c_shader::use();

        color.set();
        uv_min.set();
        uv_max.set();
    }

    void c_drop_shadow_shader::set_constants(const constants_t& constants) {
        color.value() = constants.color;

        uv_min.value().x = constants.uv_limits.min.x;
        uv_min.value().y = 1.f - constants.uv_limits.max.y;

        uv_max.value().x = constants.uv_limits.max.x;
        uv_max.value().y = 1.f - constants.uv_limits.min.y;
    }
}
#include "bliting.h"

namespace null::rml::opengl3 {
    void c_bliting_shader::create() {
        if(!empty()) return;
        c_shader::create();

        uv_min.get_location(program.get(), "uv_min");
        uv_max.get_location(program.get(), "uv_max");
    }

    void c_bliting_shader::use() {
        if(empty()) return;
        c_shader::use();

        uv_min.set();
        uv_max.set();
    }

    void c_bliting_shader::set_constants(const constants_t& constants) {
        uv_min.value().x = constants.uv_limits.min.x;
        uv_min.value().y = 1.f - constants.uv_limits.max.y;

        uv_max.value().x = constants.uv_limits.max.x;
        uv_max.value().y = 1.f - constants.uv_limits.min.y;
    }
}
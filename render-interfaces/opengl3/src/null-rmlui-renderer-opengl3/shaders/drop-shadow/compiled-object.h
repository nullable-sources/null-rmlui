#pragma once
#include <null-render-backend-opengl3/shaders/passthrough/compiled-object.h>

namespace ntl::rml::opengl3 {
    class c_drop_shadow_shader_object : public render::opengl3::c_fragment_shader {
    public:
        mem::resource_t get_source() override {
            return mem::resource_t("null-rml:opengl:shaders:drop-shadow.frag", "null-rml:opengl:resources");
        }
    } inline drop_shadow_shader_object{ };
}
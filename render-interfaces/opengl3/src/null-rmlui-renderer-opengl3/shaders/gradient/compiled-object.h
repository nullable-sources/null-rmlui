#pragma once
#include "../color/compiled-object.h"

namespace ntl::rml::opengl3 {
    class c_gradient_shader_object : public render::opengl3::c_fragment_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-rml:opengl:shaders:gradient.frag", "null-rml:opengl:resources");
        }
    } inline gradient_shader_object{ };
}
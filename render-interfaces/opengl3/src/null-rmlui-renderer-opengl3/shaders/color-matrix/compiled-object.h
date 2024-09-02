#pragma once
#include <null-render-backend-opengl3/shaders/passthrough/compiled-object.h>

namespace ntl::rml::opengl3 {
    class c_color_matrix_shader_object : public render::opengl3::c_fragment_shader {
    public:
        mem::resource_t get_source() override {
            return mem::resource_t("null-rml:opengl:shaders:color-matrix.frag", "null-rml:opengl:resources");
        }
    } inline color_matrix_shader_object{ };
}
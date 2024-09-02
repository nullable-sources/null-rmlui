#pragma once
#include <null-render-backend-opengl3/shaders/color/compiled-object.h>

namespace ntl::rml::opengl3 {
    class c_color_vertex_shader_object : public render::opengl3::c_vertex_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-rml:opengl:shaders:color.vert", "null-rml:opengl:resources");
        }
    } inline color_vertex_shader_object{ };
}
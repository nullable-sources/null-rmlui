#pragma once
#include <null-render-backend-opengl3/wrapper/shader.h>

namespace ntl::rml::opengl3 {
    class c_blur_fragment_shader_object : public render::opengl3::c_fragment_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-rml:opengl:shaders:blur.frag", "null-rml:opengl:resources");
        }
    } inline blur_fragment_shader_object{ };

    class c_blur_vertex_shader_object : public render::opengl3::c_vertex_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-rml:opengl:shaders:blur.vert", "null-rml:opengl:resources");
        }
    } inline blur_vertex_shader_object{ };
}
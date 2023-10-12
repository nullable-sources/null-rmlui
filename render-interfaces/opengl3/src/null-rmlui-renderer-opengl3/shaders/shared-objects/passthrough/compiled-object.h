#pragma once
#include <null-render-backend-opengl3.h>

namespace null::rml::opengl3 {
    class c_passthrough_shader_object : public render::opengl3::c_vertex_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            create();
            compile(memory::resource_t{ "null-rmlui-renderer:opengl:shaders:passthrough.vert", "null-rmlui-renderer:opengl:resources" }.load());
        }

        void on_destroy() override { destroy(); }

    public:
        bool empty() const override { return shader == 0; }
    } inline passthrough_shader_object{ };
}
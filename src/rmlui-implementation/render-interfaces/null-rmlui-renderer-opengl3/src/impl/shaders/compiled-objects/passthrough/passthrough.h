#pragma once
#include <wrapper/shader/shader.h>
#include <backend/shaders/shader.h>

namespace null::rml::renderer::opengl3::impl::shaders::compiled_objects {
    class c_passthrough : public render::backend::opengl3::wrapper::c_vertex_shader, public render::backend::shaders::i_compiled_object {
    public:
        void on_create() override {
            if(!empty()) return;
            create();
            compile(memory::resource_t{ "null-rmlui-renderer:opengl:shaders:passthrough.vert", "null-rmlui-renderer:opengl:resources" }.load());
        }

        void on_destroy() override { destroy(); }

    public:
        bool empty() const override { return shader == 0; }
    } inline passthrough{ };
}
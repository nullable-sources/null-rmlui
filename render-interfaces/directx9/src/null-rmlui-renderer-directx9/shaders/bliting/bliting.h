#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/bliting.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "compiled-object.h"

namespace ntl::rml::directx9 {
    class c_bliting_shader : public renderer::i_bliting_shader, public render::directx9::c_shader {
    public:
        c_bliting_shader() : c_shader(&bliting_shader_object, &render::directx9::passthrough_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
    };
}
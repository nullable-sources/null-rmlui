#pragma once
#include <null-render-backend-directx11/internal/shader.h>
#include "compiled-object.h"

namespace ntl::rml::directx11 {
    class c_drop_shadow_shader : public renderer::i_drop_shadow_shader, public render::directx11::c_shader {
    public:
        c_drop_shadow_shader() : c_shader(&drop_shadow_shader_object, &render::directx11::passthrough_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
    };
}
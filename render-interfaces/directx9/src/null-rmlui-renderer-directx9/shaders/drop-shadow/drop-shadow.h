#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/drop-shadow.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "compiled-object.h"

namespace null::rml::directx9 {
    class c_drop_shadow_shader : public renderer::i_drop_shadow_shader, public render::directx9::c_shader {
    public:
        c_drop_shadow_shader() : c_shader(&drop_shadow_shader_object, &render::directx9::passthrough_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
    };
}
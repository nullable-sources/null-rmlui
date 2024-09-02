#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/texture.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "../color/compiled-object.h"

namespace ntl::rml::directx9 {
    class c_texture_shader : public renderer::i_texture_shader, public render::directx9::c_default_shader {
    public:
        c_texture_shader() : c_default_shader(&render::directx9::texture_shader_object, &color_shader_object) { }
    };
}
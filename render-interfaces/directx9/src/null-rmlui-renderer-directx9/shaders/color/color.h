#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/color.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "compiled-object.h"

namespace ntl::rml::directx9 {
    class c_color_shader : public renderer::i_color_shader, public render::directx9::c_default_shader {
    public:
        c_color_shader() : c_default_shader(&render::directx9::color_pixel_shader_object, &color_shader_object) { }
    };
}
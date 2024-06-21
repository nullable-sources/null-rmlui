#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/color.h>
#include <null-render-backend-directx11/internal/shader.h>
#include <null-render-backend-directx11/shaders/color/compiled-object.h>
#include "compiled-object.h"

namespace null::rml::directx11 {
    class c_color_shader : public renderer::i_color_shader, public render::directx11::c_shader {
    public:
        c_color_shader() : c_shader(&render::directx11::color_pixel_shader_object, &color_shader_object) { }

    public:
        void use() override;
    };
}
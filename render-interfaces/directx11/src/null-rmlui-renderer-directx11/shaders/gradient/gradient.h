#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/gradient.h>
#include <null-render-backend-directx11/internal/shader.h>
#include "../color/compiled-object.h"
#include "compiled-object.h"

namespace null::rml::directx11 {
    class c_gradient_shader : public renderer::i_gradient_shader, public render::directx11::c_shader {
    public:
        c_gradient_shader() : c_shader(&gradient_shader_object, &color_shader_object) { }

    public:
        void use() override;

    public:
        void set_constants(const constants_t& constants) override;
    };
}
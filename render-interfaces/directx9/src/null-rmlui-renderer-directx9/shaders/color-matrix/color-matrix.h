#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/color-matrix.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "compiled-object.h"

namespace ntl::rml::directx9 {
    class c_color_matrix_shader : public renderer::i_color_matrix_shader, public render::directx9::c_shader {
    public:
        c_color_matrix_shader() : c_shader(&color_matrix_shader_object, &render::directx9::passthrough_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
    };
}
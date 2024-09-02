#pragma once
#include <null-render-backend-directx11/internal/shader.h>
#include "compiled-object.h"

namespace ntl::rml::directx11 {
    class c_blur_shader : public renderer::i_blur_shader, public render::directx11::c_shader {
    public:
        c_blur_shader() : c_shader(&blur_pixel_shader_object, &blur_vertex_shader_object) { }

    public:
        void set_constants(const constants_t& constants) override;
    };
}
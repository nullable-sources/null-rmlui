#pragma once
#include <null-render-backend-directx9/shaders/passthrough/compiled-object.h>

namespace ntl::rml::directx9 {
    namespace sources {
        static const std::vector<byte>& color_matrix() {
#include "compiled/color-matrix.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_color_matrix_shader_object : public render::directx9::c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::color_matrix().data(); }
    } inline color_matrix_shader_object{ };
}
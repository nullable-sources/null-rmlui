#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/color-matrix.h>
#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>

namespace null::rml::directx11 {
    namespace sources {
        static const std::vector<byte>& color_matrix() {
#include "compiled/color-matrix.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_color_matrix_shader_object : public render::directx11::c_pixel_shader {
    public:
        render::directx11::c_constant_buffer<renderer::i_color_matrix_shader::constants_t> constant_buffer{ };

    public:
        void set_constant(const renderer::i_color_matrix_shader::constants_t& constant, int slot = 0) {
            constant_buffer.edit_constant(constant);
            set_constant_buffer(constant_buffer.buffer, slot);
        }

    public:
        void create() override {
            if(!empty()) return;
            compile(sources::color_matrix());
            constant_buffer.create();
        }

        void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
    } inline color_matrix_shader_object{ };
}
#pragma once
#include <null-render-backend-directx11/wrapper/shaders/pixel.h>
#include <null-render-backend-directx11/wrapper/shaders/vertex.h>

namespace ntl::rml::directx11 {
    namespace sources {
        static const std::vector<byte>& color() {
#include "compiled/color.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_color_shader_object : public render::directx11::c_vertex_shader {
    public:
        struct constant_buffer_t {
        public:
            matrix4x4_t matrix{ };
            vec2_t<float> translation{ };
            float depth{ };
        };
        render::directx11::c_constant_buffer<constant_buffer_t> constant_buffer{ };

    public:
        void set_constant(const constant_buffer_t& constant, int slot = 0) {
            constant_buffer.edit_constant(constant);
            set_constant_buffer(constant_buffer.buffer, slot);
        }

    public:
        void create() override {
            if(!empty()) return;
            compile(sources::color());
            constant_buffer.create();
        }

        void destroy() override { c_vertex_shader::destroy(); constant_buffer.destroy(); }
    } inline color_shader_object{ };
}
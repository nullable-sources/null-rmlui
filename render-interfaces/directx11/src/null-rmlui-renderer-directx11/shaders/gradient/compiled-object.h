#pragma once
#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>

namespace null::rml::directx11 {
    namespace sources {
        static const std::vector<byte>& gradient() {
#include "compiled/gradient.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_gradient_shader_object : public render::directx11::c_pixel_shader {
    public:
        struct constant_buffer_t {
        public:
            std::array<color_t<float>, 16> colors{ };
            std::array<vec4_t<float>, 16> stops{ };
            vec2_t<float> p{ }, v{ };
            int func{ }, num_stops{ };
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
            compile(sources::gradient());
            constant_buffer.create();
        }

        void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
    } inline gradient_shader_object{ };
}
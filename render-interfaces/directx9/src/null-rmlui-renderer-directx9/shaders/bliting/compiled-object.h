#pragma once
#include <null-render-backend-directx9/shaders/passthrough/compiled-object.h>

namespace null::rml::directx9 {
    namespace sources {
        static const std::vector<byte>& bliting() {
#include "compiled/bliting.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_bliting_shader_object : public render::directx9::c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::bliting().data(); }
    } inline bliting_shader_object{ };
}
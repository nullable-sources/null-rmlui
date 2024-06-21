#pragma once
#include <null-render-backend-directx9/wrapper/pixel/pixel.h>

namespace null::rml::directx9 {
    namespace sources {
        static const std::vector<byte>& blend_mask() {
#include "compiled/blend-mask.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_blend_mask_shader_object : public render::directx9::c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::blend_mask().data(); }
    } inline blend_mask_shader_object{ };
}
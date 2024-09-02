#pragma once
#include <null-render-backend-directx9/wrapper/vertex/vertex.h>
#include <null-render-backend-directx9/wrapper/pixel/pixel.h>

namespace ntl::rml::directx9 {
    namespace sources {
        static const std::vector<byte>& color() {
#include "compiled/color.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_color_shader_object : public render::directx9::c_vertex_shader {
    public:
        virtual const byte* get_source() override { return sources::color().data(); }
    } inline color_shader_object{ };
}
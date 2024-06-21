#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/color.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "compiled-object.h"

namespace null::rml::opengl3 {
    class c_color_shader : public renderer::i_color_shader, public render::opengl3::c_default_shader {
    public:
        c_color_shader() : c_default_shader(&render::opengl3::color_fragment_shader_object, &color_vertex_shader_object) { }
    };
}
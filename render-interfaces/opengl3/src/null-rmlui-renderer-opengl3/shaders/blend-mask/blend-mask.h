#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/blend-mask.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "compiled-object.h"

namespace ntl::rml::opengl3 {
    class c_blend_mask_shader : public renderer::i_blend_mask_shader, public render::opengl3::c_shader {
    public:
        c_blend_mask_shader() : c_shader(&blend_mask_shader_object, &render::opengl3::passthrough_vertex_shader_object) { }
    };
}
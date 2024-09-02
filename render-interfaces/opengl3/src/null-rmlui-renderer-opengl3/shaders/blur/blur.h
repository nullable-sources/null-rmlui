#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/blur.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "compiled-object.h"

namespace ntl::rml::opengl3 {
    class c_blur_shader : public renderer::i_blur_shader, public render::opengl3::c_shader {
    public:
        render::opengl3::c_uniform<vec2_t<float>> texel_offset{ };
        render::opengl3::c_uniform<std::array<float, _num_weights>> weights{ };
        render::opengl3::c_uniform<vec2_t<float>> uv_min{ };
        render::opengl3::c_uniform<vec2_t<float>> uv_max{ };

    public:
        c_blur_shader() : c_shader(&blur_fragment_shader_object, &blur_vertex_shader_object) { }

    public:
        void create() override;

        void use() override;

    public:
        void set_constants(const constants_t& constants) override;
    };
}
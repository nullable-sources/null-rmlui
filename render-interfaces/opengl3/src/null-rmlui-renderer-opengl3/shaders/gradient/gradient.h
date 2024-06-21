#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/gradient.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "compiled-object.h"

namespace null::rml::opengl3 {
    class c_gradient_shader : public renderer::i_gradient_shader, public render::opengl3::c_default_shader {
    public:
        render::opengl3::c_uniform<int> function{ }, num_stops{ };
        render::opengl3::c_uniform<vec2_t<float>> p{ }, v{ };
        render::opengl3::c_uniform<std::array<vec4_t<float>, 16>> colors{ };
        render::opengl3::c_uniform<std::array<float, 16>> stops{ };

    public:
        c_gradient_shader() : c_default_shader(&gradient_shader_object, &color_vertex_shader_object) { }

    public:
        void create() override;

        void use() override;

    public:
        void set_constants(const constants_t& constants) override;
    };
}
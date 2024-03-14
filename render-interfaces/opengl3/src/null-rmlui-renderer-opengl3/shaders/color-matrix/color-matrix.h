#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/color-matrix.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "compiled-object.h"

namespace null::rml::opengl3 {
	class c_color_matrix_shader : public renderer::i_color_matrix_shader, public render::opengl3::c_shader {
	public:
		render::opengl3::c_uniform<matrix4x4_t> color_matrix{ };

	public:
		c_color_matrix_shader() : c_shader(&color_matrix_shader_object, &render::opengl3::passthrough_vertex_shader_object) { }

	public:
		void create() override;

		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}
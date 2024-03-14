#pragma once
#include <null-render-backend-directx11/internal/shader.h>
#include "compiled-object.h"

namespace null::rml::directx11 {
	class c_color_matrix_shader : public renderer::i_color_matrix_shader, public render::directx11::c_shader {
	public:
		c_color_matrix_shader() : c_shader(&color_matrix_shader_object, &render::directx11::passthrough_vertex_shader_object) { }

	public:
		void set_constants(const constants_t& constants) override;
	};
}
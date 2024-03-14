#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/drop-shadow.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "compiled-object.h"

namespace null::rml::opengl3 {
	class c_drop_shadow_shader : public renderer::i_drop_shadow_shader, public render::opengl3::c_shader {
	public:
		render::opengl3::c_uniform<vec4_t<float>> color{ };
		render::opengl3::c_uniform<vec2_t<float>> uv_min{ };
		render::opengl3::c_uniform<vec2_t<float>> uv_max{ };

	public:
		c_drop_shadow_shader() : c_shader(&drop_shadow_shader_object, &render::opengl3::passthrough_vertex_shader_object) { }

	public:
		void create() override;
		
		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}
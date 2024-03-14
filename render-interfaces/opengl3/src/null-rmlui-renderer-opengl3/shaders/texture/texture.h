#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/texture.h>
#include <null-render-backend-opengl3/internal/shader/shader.h>
#include "../color/compiled-object.h"
#include <null-render-backend-opengl3/shaders/texture/compiled-object.h>

namespace null::rml::opengl3 {
	class c_texture_shader : public renderer::i_texture_shader, public render::opengl3::c_default_shader {
	public:
		c_texture_shader() : c_default_shader(&render::opengl3::texture_shader_object, &color_vertex_shader_object) { }
	};
}
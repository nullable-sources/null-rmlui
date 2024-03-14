#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/texture.h>
#include <null-render-backend-directx11/internal/shader.h>
#include <null-render-backend-directx11/shaders/texture/compiled-object.h>
#include "../color/compiled-object.h"

namespace null::rml::directx11 {
	class c_texture_shader : public renderer::i_texture_shader, public render::directx11::c_shader {
	public:
		c_texture_shader() : c_shader(&render::directx11::texture_shader_object, &color_shader_object) { }

	public:
		void use() override;
	};
}
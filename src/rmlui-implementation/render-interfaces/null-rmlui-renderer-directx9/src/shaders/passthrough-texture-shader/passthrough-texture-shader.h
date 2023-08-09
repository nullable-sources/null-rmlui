#pragma once
#include <null-rmlui.h>
#include "shaders/shared-compiled-objects/passthrough-object/compiled-object.h"

namespace null::rml::directx9 {
	class c_passthrough_texture_shader : public renderer::i_passthrough_texture_shader, public render::directx9::c_shader {
	public:
		Rml::Vector2f translation{ };

	public:
		c_passthrough_texture_shader() : c_shader{ &render::directx9::passthrough_texture_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;

	public:
		void set_translation(const Rml::Vector2f& _translation) { translation = _translation; }
	};
}
#pragma once
#include <null-rmlui.h>
#include "../shared-objects/passthrough/compiled-object.h"

namespace null::rml::directx11 {
	class c_passthrough_texture_shader : public renderer::i_passthrough_texture_shader, public render::directx11::c_shader {
	public:
		c_passthrough_shader_object::constant_buffer_t constant{ };

	public:
		c_passthrough_texture_shader() : c_shader(&render::directx11::passthrough_texture_shader_object, &passthrough_shader_object) { }

	public:
		void use() override;

	public:
		void set_translation(const Rml::Vector2f& translation) override { constant.translation = translation; }
	};
}
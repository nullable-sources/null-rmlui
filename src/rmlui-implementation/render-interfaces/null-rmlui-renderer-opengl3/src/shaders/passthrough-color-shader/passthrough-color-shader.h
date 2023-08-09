#pragma once
#include <null-rmlui.h>
#include "shaders/shared-compiled-objects/passthrough-object/compiled-object.h"

namespace null::rml::opengl3 {
	class c_passthrough_color_shader : public renderer::i_passthrough_color_shader, public render::opengl3::c_shader {
	public:
		render::opengl3::c_uniform<matrix4x4_t> matrix{ };
		render::opengl3::c_uniform<Rml::Vector2f> translation{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_translation(const Rml::Vector2f& _translation) override { translation.value() = _translation; }
	};
}
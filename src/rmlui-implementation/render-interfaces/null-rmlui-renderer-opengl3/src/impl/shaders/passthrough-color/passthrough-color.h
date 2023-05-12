#pragma once
#include <null-render-backend-opengl3.h>
#include <rmlui-implementation/render-interface/impl/shaders/passthrough.h>

namespace null::rml::renderer::opengl3::impl::shaders {
	class c_passthrough_color : public renderer::impl::shaders::i_passthrough_color, public render::backend::opengl3::shaders::i_shader {
	public:
		render::backend::opengl3::wrapper::c_uniform<matrix4x4_t> matrix{ };
		render::backend::opengl3::wrapper::c_uniform<Rml::Vector2f> translation{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_translation(const Rml::Vector2f& _translation) override { translation.value() = _translation; }
	};
}
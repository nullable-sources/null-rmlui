#pragma once
#include <shaders/shader.h>
#include <rmlui-implementation/render-interface/impl/shaders/passthrough.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <impl/shaders/compiled-objects/passthrough/passthrough.h>

namespace null::rml::renderer::directx9::impl::shaders {
	class c_passthrough_texture : public renderer::impl::shaders::i_passthrough_texture, public render::backend::directx9::shaders::i_shader {
	public:
		Rml::Vector2f translation{ };

	public:
		c_passthrough_texture() : i_shader{ &render::backend::directx9::shaders::compiled_objects::passthrough_texture, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			vertex_shader->set_constant(0, render::backend::renderer->get_matrix().linear_array.data(), 4);
			vertex_shader->set_constant(2, (float*)translation, 1);
		}

	public:
		void set_translation(const Rml::Vector2f& _translation) { translation = _translation; }
	};
}
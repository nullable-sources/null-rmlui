#pragma once
#include <shaders/shader.h>
#include <rmlui-implementation/render-interface/impl/shaders/passthrough.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <impl/shaders/compiled-objects/passthrough/passthrough.h>

namespace null::rml::renderer::directx11::impl::shaders {
	class c_passthrough_texture : public renderer::impl::shaders::i_passthrough_texture, public render::backend::directx11::shaders::i_shader {
	public:
		compiled_objects::c_passthrough::constant_buffer_t constant{ };

	public:
		c_passthrough_texture() : i_shader{ &render::backend::directx11::shaders::compiled_objects::passthrough_texture, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			constant.matrix = render::backend::renderer->get_matrix();
			compiled_objects::passthrough.set_constant(constant);
		}

	public:
		void set_translation(const Rml::Vector2f& translation) override { constant.translation = translation; }
	};
}
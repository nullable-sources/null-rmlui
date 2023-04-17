#pragma once
#include <null-backend-opengl3.h>
#include <rmlui-implementation/render-interface/impl/shaders/passthrough.h>
#include <impl/shaders/compiled-objects/passthrough/passthrough.h>

namespace null::rml::renderer::opengl3::impl::shaders {
	class c_passthrough_texture : public renderer::impl::shaders::i_passthrough_texture, public render::backend::opengl3::shaders::i_shader{
	public:
		render::backend::opengl3::wrapper::c_uniform<matrix4x4_t> matrix{ };
		render::backend::opengl3::wrapper::c_uniform<Rml::Vector2f> translation{ };

	public:
		void on_create() override {
			if(!empty()) return;
			program = std::make_unique<render::backend::opengl3::wrapper::c_program>();
			program->create();

			program->attach_shader(&render::backend::opengl3::shaders::compiled_objects::passthrough_texture);
			program->attach_shader(&compiled_objects::passthrough);

			program->link();

			program->detach_shader(&render::backend::opengl3::shaders::compiled_objects::passthrough_texture);
			program->detach_shader(&compiled_objects::passthrough);

			matrix.get_location(program.get(), "matrix");
			translation.get_location(program.get(), "translation");
		}

		void on_destroy() override {
			program->destroy();
		}

		void use() override {
			program->use();

			matrix.set(render::backend::renderer->get_matrix());
			opengl::uniform2f(translation.location, translation.value().x, translation.value().y);
		}

	public:
		void set_translation(const Rml::Vector2f& _translation) override { translation.value() = _translation; }
	};
}
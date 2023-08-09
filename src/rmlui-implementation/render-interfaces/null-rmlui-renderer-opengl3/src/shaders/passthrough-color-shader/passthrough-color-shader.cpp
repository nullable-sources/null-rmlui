#include "shaders/passthrough-color-shader/passthrough-color-shader.h"

namespace null::rml::opengl3 {
	void c_passthrough_color_shader::on_create() {
		if(!empty()) return;
		program = std::make_unique<render::opengl3::c_program>();
		program->create();

		program->attach_shader(&render::opengl3::passthrough_color_shader_object);
		program->attach_shader(&passthrough_shader_object);

		program->link();

		program->detach_shader(&render::opengl3::passthrough_color_shader_object);
		program->detach_shader(&passthrough_shader_object);

		matrix.get_location(program.get(), "matrix");
		translation.get_location(program.get(), "translation");
	}

	void c_passthrough_color_shader::on_destroy() {
		program->destroy();
	}

	void c_passthrough_color_shader::use() {
		program->use();

		matrix.set(render::backend::renderer->get_matrix());
		opengl::uniform2f(translation.location, translation.value().x, translation.value().y);
	}
}
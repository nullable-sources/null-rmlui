#include <impl/shaders/passthrough-color/passthrough-color.h>
#include <impl/shaders/compiled-objects/passthrough/passthrough.h>
#include <shaders/compiled-objects/passthrough-color/passthrough-color.h>
#include <rmlui-implementation/render-interface/render-interface.h>

namespace null::rml::renderer::opengl3::impl::shaders {
	void c_passthrough_color::on_create() {
		if(!empty()) return;
		program = std::make_unique<render::backend::opengl3::wrapper::c_program>();
		program->create();

		program->attach_shader(&render::backend::opengl3::shaders::compiled_objects::passthrough_color);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&render::backend::opengl3::shaders::compiled_objects::passthrough_color);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");
		translation.get_location(program.get(), "translation");
	}

	void c_passthrough_color::on_destroy() {
		program->destroy();
	}

	void c_passthrough_color::use() {
		program->use();

		matrix.set(render::backend::renderer->get_matrix());
		opengl::uniform2f(translation.location, translation.value().x, translation.value().y);
	}
}
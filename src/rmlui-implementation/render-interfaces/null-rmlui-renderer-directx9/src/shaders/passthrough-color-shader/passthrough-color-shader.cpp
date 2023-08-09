#include "shaders/passthrough-color-shader/passthrough-color-shader.h"

namespace null::rml::directx9 {
	void c_passthrough_color_shader::use() {
		if (empty()) return;
		c_shader::use();

		vertex_shader->set_constant(0, render::backend::renderer->get_matrix().linear_array.data(), 4);
		vertex_shader->set_constant(2, (float*)translation, 1);
	}
}
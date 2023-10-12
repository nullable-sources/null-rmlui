#include "passthrough-color.h"

namespace null::rml::directx11 {
	void c_passthrough_color_shader::use() {
		if (empty()) return;
		c_shader::use();

		constant.matrix = render::backend::renderer->get_matrix();
		passthrough_shader_object.set_constant(constant);
	}
}
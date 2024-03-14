#include "color.h"

namespace null::rml::directx11 {
	void c_color_shader::use() {
		if(empty()) return;
		c_shader::use();

		color_shader_object.set_constant({ null::render::backend::renderer->get_matrix(), null::render::backend::renderer->get_translation() });
	}
}
#include "gradient.h"

namespace null::rml::directx11 {
	void c_gradient_shader::use() {
		if(empty()) return;
		c_shader::use();

		color_shader_object.set_constant({ null::render::backend::renderer->get_matrix(), null::render::backend::renderer->get_translation() });
	}

	void c_gradient_shader::set_constants(const constants_t& constants) {
		c_gradient_shader_object::constant_buffer_t constant_buffer{ };

		for(auto [color, stop, pair] : std::views::zip(constant_buffer.colors, constant_buffer.stops, constants.stops)) {
			color = pair.first;
			stop.x = pair.second;
		}

		constant_buffer.p = constants.p;
		constant_buffer.v = constants.v;
		constant_buffer.func = (int)constants.function;
		constant_buffer.num_stops = constants.stops.size();

		gradient_shader_object.set_constant(constant_buffer);
	}
}
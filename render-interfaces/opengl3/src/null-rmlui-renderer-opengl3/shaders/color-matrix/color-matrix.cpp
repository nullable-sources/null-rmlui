#include "color-matrix.h"

namespace null::rml::opengl3 {
	void c_color_matrix_shader::create() {
		if(!empty()) return;
		c_shader::create();

		color_matrix.get_location(program.get(), "color_matrix");
	}

	void c_color_matrix_shader::use() {
		if(empty()) return;
		c_shader::use();

		color_matrix.set();
	}

	void c_color_matrix_shader::set_constants(const constants_t& constants) {
		color_matrix.value() = constants.color_matrix;
	}
}
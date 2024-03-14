#include "bliting.h"

namespace null::rml::directx11 {
	void c_bliting_shader::set_constants(const constants_t& constants) {
		bliting_shader_object.set_constant(constants);
	}
}
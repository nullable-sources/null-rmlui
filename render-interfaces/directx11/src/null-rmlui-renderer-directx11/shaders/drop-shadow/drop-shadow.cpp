#include "drop-shadow.h"

namespace null::rml::directx11 {
	void c_drop_shadow_shader::set_constants(const constants_t& constants) {
		drop_shadow_shader_object.set_constant(constants);
	}
}
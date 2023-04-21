#pragma once
#include <extensions/decorators/checkmark/checkmark.h>
#include <extensions/decorators/linear-gradient/linear-gradient.h>
#include <extensions/decorators/quad-gradient/quad-gradient.h>

namespace null::rml::extensions::decorators {
	static void register_all() {
		c_checkmark::register_instancer();
		c_linear_gradient::register_instancer();
		c_quad_gradient::register_instancer();
	}
}
#pragma once
#include <extensions/decorators/checkmark/checkmark.h>

namespace null::rml::extensions::decorators {
	static void register_all() {
		c_checkmark::register_instancer();
	}
}
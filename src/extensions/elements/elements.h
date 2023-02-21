#pragma once
#include <extensions/elements/multiselect/multiselect.h>

namespace null::rml::extensions::elements {
	static void register_all() {
		c_element_form_control_multiselect::register_instancer();
	}
}
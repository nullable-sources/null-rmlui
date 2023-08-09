#pragma once
#include "extensions/checkmark-decorator/checkmark-decorator.h"
#include "extensions/linear-gradient-decorator/linear-gradient-decorator.h"
#include "extensions/quad-gradient-decorator/quad-gradient-decorator.h"

#include "extensions/multiselect-element/multiselect-element.h"
#include "extensions/colorpicker-element/colorpicker-element.h"

namespace null::rml::extensions {
	static void register_all_decorators() {
		c_checkmark::register_instancer();
		c_linear_gradient::register_instancer();
		c_quad_gradient::register_instancer();
	}

	static void register_all_elements() {
		c_element_form_control_multiselect::register_instancer();
	}

	static void register_all() {
		register_all_decorators();
		register_all_elements();
	}
}
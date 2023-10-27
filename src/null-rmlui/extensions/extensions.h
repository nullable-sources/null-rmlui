#pragma once
#include "linear-gradient-decorator/linear-gradient-decorator.h"
#include "quad-gradient-decorator/quad-gradient-decorator.h"

#include "multiselect-element/multiselect-element.h"
#include "colorpicker-element/colorpicker-element.h"

namespace null::rml::extensions {
	static void register_all_decorators() {
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
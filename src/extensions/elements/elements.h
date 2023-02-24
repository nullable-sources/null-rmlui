#pragma once
#include <extensions/elements/multiselect/multiselect.h>
#include <extensions/elements/colorpicker/colorpicker.h>
#include <extensions/elements/colorpicker-box/colorpicker-box.h>

namespace null::rml::extensions::elements {
	static void register_all() {
		c_element_form_control_multiselect::register_instancer();
		c_element_form_control_colorpicker_box::register_instancer();
		c_element_form_control_colorpicker::register_instancer();
	}
}
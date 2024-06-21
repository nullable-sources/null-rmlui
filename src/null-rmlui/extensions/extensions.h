#pragma once
#include "multiselect-element/multiselect-element.h"
#include "colorpicker-element/colorpicker-element.h"

namespace null::rml::extensions {
    static void register_all_elements() {
        c_element_form_control_multiselect::register_instancer();
    }

    static void register_all() {
        register_all_elements();
    }
}
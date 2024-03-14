#pragma once
#include <null-render-backend-opengl3/shaders/passthrough/compiled-object.h>

namespace null::rml::opengl3 {
	class c_bliting_shader_object : public render::opengl3::c_fragment_shader {
	public:
		memory::resource_t get_source() override {
			return memory::resource_t("null-rml:opengl:shaders:bliting.frag", "null-rml:opengl:resources");
		}
	} inline bliting_shader_object{ };
}
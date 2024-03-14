#pragma once
#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>

namespace null::rml::directx11 {
	namespace sources {
		static const std::vector<byte>& blend_mask() {
#include "compiled/blend-mask.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_blend_mask_shader_object : public render::directx11::c_pixel_shader {
	public:
		void create() override {
			if(!empty()) return;
			compile(sources::blend_mask());
		}
	} inline blend_mask_shader_object{ };
}
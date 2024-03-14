#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/drop-shadow.h>
#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>

namespace null::rml::directx11 {
	namespace sources {
		static const std::vector<byte>& drop_shadow() {
#include "compiled/drop-shadow.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_drop_shadow_shader_object : public render::directx11::c_pixel_shader {
	public:
		render::directx11::c_constant_buffer<renderer::i_drop_shadow_shader::constants_t> constant_buffer{ };

	public:
		void set_constant(const renderer::i_drop_shadow_shader::constants_t& constant, int slot = 0) {
			constant_buffer.edit_constant(constant);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

	public:
		void create() override {
			if(!empty()) return;
			compile(sources::drop_shadow());
			constant_buffer.create();
		}

		void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
	} inline drop_shadow_shader_object{ };
}
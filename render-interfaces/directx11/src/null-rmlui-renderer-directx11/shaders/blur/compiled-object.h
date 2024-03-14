#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/blur.h>
#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>

namespace null::rml::directx11 {
	namespace sources {
		static const std::vector<byte>& blur_ps() {
#include "compiled/blur-ps.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}

		static const std::vector<byte>& blur_vs() {
#include "compiled/blur-vs.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_blur_pixel_shader_object : public render::directx11::c_pixel_shader {
	public:
		struct constant_buffer_t {
		public:
			rect_t<float> uv_limits{ };
			std::array<vec4_t<float>, renderer::i_blur_shader::_num_weights> weights{ };
		};
		render::directx11::c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void set_constant(const constant_buffer_t& constant, int slot = 0) {
			constant_buffer.edit_constant(constant);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

	public:
		void create() override {
			if(!empty()) return;
			compile(sources::blur_ps());
			constant_buffer.create();
		}

		void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
	} inline blur_pixel_shader_object{ };

	class c_blur_vertex_shader_object : public render::directx11::c_vertex_shader {
	public:
		struct constant_buffer_t {
		public:
			vec2_t<float> texel_offset{ };
		};
		render::directx11::c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void set_constant(const constant_buffer_t& constant, int slot = 0) {
			constant_buffer.edit_constant(constant);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

	public:
		void create() override {
			if(!empty()) return;
			compile(sources::blur_vs());
			constant_buffer.create();
		}

		void destroy() override { c_vertex_shader::destroy(); constant_buffer.destroy(); }
	} inline blur_vertex_shader_object{ };
}
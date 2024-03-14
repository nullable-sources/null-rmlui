#pragma once
#include <null-render-backend-directx9/shaders/passthrough/compiled-object.h>

namespace null::rml::directx9 {
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

	class c_blur_pixel_shader_object : public render::directx9::c_pixel_shader {
	public:
		virtual const byte* get_source() override { return sources::blur_ps().data(); }
	} inline blur_pixel_shader_object{ };

	class c_blur_vertex_shader_object : public render::directx9::c_vertex_shader {
	public:
		virtual const byte* get_source() override { return sources::blur_vs().data(); }
	} inline blur_vertex_shader_object{ };
}
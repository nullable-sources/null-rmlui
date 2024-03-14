#pragma once
#include <null-render/backend/internal/shader.h>
#include <null-render/backend/renderer/renderer.h>

namespace null::rml::renderer {
	class i_bliting_shader : public virtual render::backend::i_shader {
	public:
		struct constants_t {
		public:
			rect_t<float> uv_limits{ };

		public:
			void from_region(const rect_t<float>& region) { uv_limits = (region + rect_t<float>(0.5f, -0.5f)) / (vec2_t<float>)render::shared::viewport; }
		};

	public:
		virtual void set_constants(const constants_t& constants) = 0;
	}; inline std::unique_ptr<i_bliting_shader> bliting_shader{ };
}
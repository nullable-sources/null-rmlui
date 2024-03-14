#pragma once
#include "bliting.h"

namespace null::rml::renderer {
	class i_blur_shader : public virtual render::backend::i_shader {
	public:
		constexpr static int _blur_size = 7;
		constexpr static int _num_weights = ((_blur_size + 1) / 2);

		struct constants_t : public i_bliting_shader::constants_t {
		public:
			vec2_t<float> texel_offset{ };
			std::vector<float> weights{ };
		};

	public:
		virtual void set_constants(const constants_t& constants) = 0;
	}; inline std::unique_ptr<i_blur_shader> blur_shader{ };
}
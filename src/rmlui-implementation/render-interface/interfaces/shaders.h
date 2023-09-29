#pragma once
#include <RmlUi/Include/RmlUi/Core/Types.h>
#include <null-render.h>

namespace null::rml::renderer {
	class i_passthrough_color_shader : public virtual render::backend::i_shader {
	public:
		virtual void set_translation(const Rml::Vector2f& translation) = 0;
	}; inline std::unique_ptr<i_passthrough_color_shader> passthrough_color_shader{ };

	class i_passthrough_texture_shader : public virtual render::backend::i_shader {
	public:
		virtual void set_translation(const Rml::Vector2f& translation) = 0;
	}; inline std::unique_ptr<i_passthrough_texture_shader> passthrough_texture_shader{ };
}
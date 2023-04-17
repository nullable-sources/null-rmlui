#pragma once
#include <RmlUi/Core/Types.h>
#include <backend/shaders/shader.h>

namespace null::rml::renderer::impl::shaders {
	class i_passthrough_color : public virtual render::backend::shaders::i_shader {
	public:
		virtual void set_translation(const Rml::Vector2f& translation) = 0;
	}; inline std::unique_ptr<i_passthrough_color> passthrough_color{ };

	class i_passthrough_texture : public virtual render::backend::shaders::i_shader {
	public:
		virtual void set_translation(const Rml::Vector2f& translation) = 0;
	}; inline std::unique_ptr<i_passthrough_texture> passthrough_texture{ };
}
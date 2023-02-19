#pragma once
#include <null-renderer-opengl3.h>
#include <null-rmlui.h>

namespace null::rml::renderer {
	class c_opengl3 : public i_render_interface {
	public:
		bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) override;
		void ReleaseTexture(Rml::TextureHandle texture_handle) override;

	public:
		void enable_scissor_region(const bool& enable) override;
		void set_transform(const matrix4x4_t& transform) override;
	};
}
#pragma once
#include <null-renderer-directx11.h>
#include <null-rmlui.h>

namespace null::rml::renderer {
	class c_directx11 : public i_render_interface {
	public:
		ID3D11RasterizerState* disabled_scissor{ }, *enabled_scissor{ };

	public:
		~c_directx11() {
			if(disabled_scissor) { disabled_scissor->Release(); disabled_scissor = nullptr; }
			if(enabled_scissor) { enabled_scissor->Release(); enabled_scissor = nullptr; }
		}

	public:
		bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) override;
		void ReleaseTexture(Rml::TextureHandle texture_handle) override;

	public:
		void enable_scissor_region(const bool& enable) override;
		void set_transform(const matrix4x4_t& transform) override;
	};
}
#pragma once
#include <null-backend-directx11.h>
#include <null-rmlui.h>

#include <impl/mesh/mesh.h>
#include <impl/commands/commands.h>
#include <impl/shaders/passthrough-color/passthrough-color.h>
#include <impl/shaders/passthrough-texture/passthrough-texture.h>

namespace null::rml::renderer {
	class c_directx11 : public i_render_interface {
	public:
		ID3D11RasterizerState* disabled_scissor{ }, *enabled_scissor{ };

	public:
		void initialize();
		void shutdown();

	public:
		std::unique_ptr<renderer::impl::i_mesh> instance_mesh() override { return std::make_unique<directx11::impl::c_mesh>(); }
		std::unique_ptr<renderer::impl::commands::i_clip_enable> instatnce_clip_enable_command(const bool& enable) { return std::make_unique<directx11::impl::commands::c_clip_enable>(enable, enable ? enabled_scissor : disabled_scissor); }

		std::unique_ptr<renderer::impl::shaders::i_passthrough_color> instance_passthrough_color_shader() override { return std::make_unique<directx11::impl::shaders::c_passthrough_color>(); }
		std::unique_ptr<renderer::impl::shaders::i_passthrough_texture> instance_passthrough_texture_shader() override { return std::make_unique<directx11::impl::shaders::c_passthrough_texture>(); }
	};
}
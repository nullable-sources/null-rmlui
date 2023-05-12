#pragma once
#include <null-render-backend-directx9.h>
#include <null-rmlui.h>

#include <impl/mesh/mesh.h>
#include <impl/comands/commands.h>
#include <impl/shaders/passthrough-color/passthrough-color.h>
#include <impl/shaders/passthrough-texture/passthrough-texture.h>

namespace null::rml::renderer {
	class c_directx9 : public i_render_interface {
	public:
		std::unique_ptr<renderer::impl::i_mesh> instance_mesh() override { return std::make_unique<directx9::impl::c_mesh>(); }
		std::unique_ptr<renderer::impl::commands::i_clip_enable> instatnce_clip_enable_command(const bool& enable) { return std::make_unique<directx9::impl::commands::c_clip_enable>(enable); }

		std::unique_ptr<renderer::impl::shaders::i_passthrough_color> instance_passthrough_color_shader() override { return std::make_unique<directx9::impl::shaders::c_passthrough_color>(); }
		std::unique_ptr<renderer::impl::shaders::i_passthrough_texture> instance_passthrough_texture_shader() override { return std::make_unique<directx9::impl::shaders::c_passthrough_texture>(); }
	};
}
#pragma once
#include "mesh/mesh.h"
#include "commands/commands.h"
#include "shaders/passthrough-color-shader/passthrough-color-shader.h"
#include "shaders/passthrough-texture-shader/passthrough-texture-shader.h"

namespace null::rml::directx11 {
	class c_render : public i_render_interface {
	public:
		ID3D11RasterizerState* disabled_scissor{ }, *enabled_scissor{ };

	public:
		void initialize();
		void shutdown();

	public:
		std::unique_ptr<renderer::i_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::unique_ptr<renderer::i_clip_enable_command> instatnce_clip_enable_command(bool enable) { return std::make_unique<c_clip_enable_command>(enable, enable ? enabled_scissor : disabled_scissor); }

		std::unique_ptr<renderer::i_passthrough_color_shader> instance_passthrough_color_shader() override { return std::make_unique<c_passthrough_color_shader>(); }
		std::unique_ptr<renderer::i_passthrough_texture_shader> instance_passthrough_texture_shader() override { return std::make_unique<c_passthrough_texture_shader>(); }
	};
}
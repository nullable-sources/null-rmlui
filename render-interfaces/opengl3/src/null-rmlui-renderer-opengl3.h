#pragma once
#include "null-rmlui-renderer-opengl3/mesh/mesh.h"
#include "null-rmlui-renderer-opengl3/commands/commands.h"
#include "null-rmlui-renderer-opengl3/shaders/passthrough-color/passthrough-color.h"
#include "null-rmlui-renderer-opengl3/shaders/passthrough-texture/passthrough-texture.h"

namespace null::rml::opengl3 {
	class c_render : public i_render_interface {
	public:
		std::unique_ptr<renderer::i_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::unique_ptr<renderer::i_clip_enable_command> instatnce_clip_enable_command(bool enable) { return std::make_unique<c_clip_enable_command>(enable); }

		std::unique_ptr<renderer::i_passthrough_color_shader> instance_passthrough_color_shader() override { return std::make_unique<c_passthrough_color_shader>(); }
		std::unique_ptr<renderer::i_passthrough_texture_shader> instance_passthrough_texture_shader() override { return std::make_unique<c_passthrough_texture_shader>(); }
	};
}
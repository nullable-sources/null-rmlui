#pragma once
#include "null-rmlui-renderer-opengl3/mesh/mesh.h"
#include "null-rmlui-renderer-opengl3/commands/commands.h"

namespace null::rml::opengl3 {
	class c_render : public i_render_interface {
	public:
		std::unique_ptr<renderer::i_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::shared_ptr<renderer::i_clip_enable_command> instatnce_clip_enable_command(bool enable) { return std::make_shared<c_clip_enable_command>(enable); }
	};
}
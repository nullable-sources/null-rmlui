#pragma once
#include <null-render-backend-opengl3.h>
#include <rmlui-implementation/render-interface/impl/commands.h>

namespace null::rml::renderer::opengl3::impl::commands {
	class c_clip_enable : public renderer::impl::commands::i_clip_enable {
	public:
		c_clip_enable(const bool& _enable) : i_clip_enable{ _enable } { }

	public:
		void handle() override { enable ? opengl::enable(opengl::e_scissor_test) : opengl::disable(opengl::e_scissor_test); }
	};
}
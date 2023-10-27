#pragma once
#include <null-render-backend-opengl3.h>
#include <null-rmlui.h>

namespace null::rml::opengl3 {
	class c_clip_enable_command : public renderer::i_clip_enable_command {
	public:
		c_clip_enable_command(bool _enable) : i_clip_enable_command(_enable) { }

	public:
		void handle() override { enable ? opengl::enable(opengl::e_scissor_test) : opengl::disable(opengl::e_scissor_test); }
	};
}